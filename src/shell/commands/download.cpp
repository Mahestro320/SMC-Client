#include <cerrno>
#include <conio.h>
#include <thread>
#include "io/console.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/commands/download.hpp"
#include "shell/config.hpp"
#include "shell/system/cursor.hpp"
#include "util/string.hpp"

namespace fs = std::filesystem;

exit_code_t DownloadCommand::run() {
    if (!init()) {
        return Error;
    }
    initQueue();
    createThreads();
    infoLoop();
    deleteFileInstance();
    deleteFileIfNeeded();
    return error_flag.load(std::memory_order_acquire) ? Error : Success;
}

bool DownloadCommand::init() {
    loadBufferSize();
    if (!getInputPath() || !getOutputPath()) {
        return false;
    }
    console::out::verbose("input path: " + input_path_real.string() + "\noutput path: " + output_path.string());
    if (!beginHandler() || !createOutputFile()) {
        return false;
    }
    return true;
}

void DownloadCommand::initQueue() {
    queue.setMaxSize(MAX_QUEUE_SIZE);
}

void DownloadCommand::createThreads() {
    createWriterThread();
    createReaderThread();
    createCancelThread();
}

void DownloadCommand::deleteFileInstance() {
    if (output_file) {
        output_file->close();
        delete output_file;
    }
}

void DownloadCommand::deleteFileIfNeeded() {
    Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    if (!config_values.cmd_download_del_file_on_stop || !stop_flag.load(std::memory_order_acquire)) {
        return;
    }
    std::error_code ec{};
    std::filesystem::remove(output_path, ec);
    if (ec) {
        console::out::err("error while removing output file: " + ec.message());
    }
}

void DownloadCommand::loadBufferSize() {
    const Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    buffer_size = values.cmd_download_buffer_size;
}

bool DownloadCommand::getInputPath() {
    return getRawInputPath() && readCompleteInputPath() && readRealInputPath();
}

bool DownloadCommand::getRawInputPath() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return false;
    }
    input_path_raw = fs::path{args[0]};
    return true;
}

bool DownloadCommand::readCompleteInputPath() {
    IOGetCompletePathRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_raw);
    if (!handler.run()) {
        return false;
    }
    if (!handler.fileExists()) {
        console::out::err("target file \"" + input_path_raw.string() + "\" does not exists");
        return false;
    }
    input_path_complete = handler.getValue();
    return true;
}

bool DownloadCommand::readRealInputPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_complete);
    if (!handler.run()) {
        return false;
    }
    input_path_real = handler.getValue();
    return true;
}

bool DownloadCommand::getOutputPath() {
    if (args.size() >= 2) {
        output_path = args[1];
        return true;
    }
    console::out::verbose("output path isn't specified, setting default value");
    const Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    try {
        output_path = values.cmd_download_output / getNewOutputFileNameInDir(values.cmd_download_output);
    } catch (const std::exception& e) {
        console::out::err(e);
        return false;
    }
    return true;
}

bool DownloadCommand::createOutputFile() {
    console::out::verbose("creating output file... ", false);
    output_file = new std::ofstream{output_path, std::ios::binary | std::ios::app};
    if (!output_file || !output_file->is_open()) {
        console::out::err("error while creating output file");
        return false;
    }
    console::out::verbose("success");
    return true;
}

std::filesystem::path DownloadCommand::getNewOutputFileNameInDir(const std::filesystem::path& path) const {
    std::filesystem::path result{};
    const std::filesystem::path input_path_no_ext{input_path_real.stem()};
    const std::filesystem::path input_path_ext{input_path_real.extension()};
    const std::string filename{input_path_no_ext.string()};
    const std::string file_ext{input_path_ext.string()};
    uint64_t i{};
    do {
        result = filename + (i == 0 ? "" : (filename.empty() ? "" : "_") + std::to_string(i + 1)) + file_ext;
        ++i;
    } while (fs::exists(path / result));
    return result;
}

bool DownloadCommand::beginHandler() {
    handler.setClient(client);
    handler.setPath(input_path_real);
    handler.setBufferSize(buffer_size);
    if (!handler.run()) {
        return false;
    }
    buffer_count = handler.getBufferCount();
    return true;
}

void DownloadCommand::infoLoop() {
    initInfoPrinter();
    startInfoLoop();
}

void DownloadCommand::initInfoPrinter() {
    info_printer.setBufferSize(buffer_size);
    info_printer.setBufferCount(buffer_count);
}

void DownloadCommand::startInfoLoop() {
    console::out::inf("downloading file, press ESC to cancel\n");
    shell::cursor::setVisible(false);
    do {
        printInfoMessage();
    } while (!transmitionFinished());
    endInfoLoop();
    shell::cursor::setVisible(true);
}

void DownloadCommand::endInfoLoop() {
    if (stop_flag.load(std::memory_order_acquire) || error_flag.load(std::memory_order_acquire)) {
        console::out::inf();
    } else {
        printInfoMessageNoWait();
    }
    if (is_cancelled_by_esc) {
        console::out::inf("operation cancelled");
    }
}

void DownloadCommand::printInfoMessage() {
    printInfoMessageNoWait();
    std::this_thread::sleep_for(std::chrono::seconds{1});
}

void DownloadCommand::printInfoMessageNoWait() {
    info_printer.setCurrBufferIndex(curr_buffer_idx.load(std::memory_order_acquire));
    info_printer.update();
    info_printer.print();
}

void DownloadCommand::createReaderThread() {
    std::thread t{[&] { readerThreadHandle(); }};
    t.detach();
}

void DownloadCommand::readerThreadHandle() {
    bool stopped{};
    for (; handler.available() && !stopped; curr_buffer_idx.fetch_add(1, std::memory_order_release)) {
        if (!downloadNextBuffer()) {
            error_flag.store(true, std::memory_order_release);
            stopped = true;
            break;
        }
        stopped = writer_finished_flag.load(std::memory_order_acquire) || stop_flag.load(std::memory_order_acquire);
    }
    if (stopped) {
        stop_flag.store(true, std::memory_order_release);
        stopHandler();
    }
    reader_finished_flag.store(true, std::memory_order_release);
}

bool DownloadCommand::downloadNextBuffer() {
    if (!handler.downloadNextBuffer()) {
        return false;
    }
    waitForFreeSpaceInQueue();
    queue.pushBack(handler.getCurrentBuffer());
    cv.notify_one();
    return true;
}

void DownloadCommand::stopHandler() {
    handler.stop();
}

void DownloadCommand::waitForFreeSpaceInQueue() {
    std::unique_lock lock(mtx);
    cv.wait(lock, [&] { return queue.size() < queue.maxSize(); });
}

void DownloadCommand::createWriterThread() {
    std::thread t{[&] { writerThreadHandle(); }};
    t.detach();
}

void DownloadCommand::writerThreadHandle() {
    while (!stop_flag.load(std::memory_order_acquire) &&
           (!reader_finished_flag.load(std::memory_order_acquire) || !queue.empty())) {
        waitForQueueToBeFilled();
        if (!writeNextBuffer()) {
            stop_flag.store(true, std::memory_order_release);
            error_flag.store(true, std::memory_order_release);
            break;
        }
    }
    writer_finished_flag.store(true, std::memory_order_release);
}

void DownloadCommand::waitForQueueToBeFilled() {
    std::unique_lock lock(mtx);
    cv.wait(lock, [&] { return !queue.empty(); });
}

bool DownloadCommand::writeNextBuffer() {
    std::vector<char> buffer{queue.getAndPopFront()};
    cv.notify_one();
    output_file->write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    if (!*output_file) {
        char buffer[1024];
        strerror_s(buffer, sizeof(buffer), errno);
        console::out::err("error while writing data into file: " + std::string{buffer});
        return false;
    }
    return true;
}

void DownloadCommand::createCancelThread() {
    std::thread t{[&] { cancelThreadHandle(); }};
    t.detach();
}

void DownloadCommand::cancelThreadHandle() {
    while (!stop_flag.load(std::memory_order_acquire) && !reader_finished_flag.load(std::memory_order_acquire) &&
           !writer_finished_flag.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        if (_kbhit() && _getch() == ESC_KEY) {
            stop_flag.store(true, std::memory_order_release);
            is_cancelled_by_esc = true;
            break;
        }
    }
    cancel_finished_flag.store(true, std::memory_order_release);
}

bool DownloadCommand::transmitionFinished() {
    return reader_finished_flag.load(std::memory_order_acquire) &&
           writer_finished_flag.load(std::memory_order_acquire) && cancel_finished_flag.load(std::memory_order_acquire);
}
