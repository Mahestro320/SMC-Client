#include <thread>
#include "io/console.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/commands/download.hpp"
#include "shell/config.hpp"
#include "util/string.hpp"

namespace fs = std::filesystem;

exit_code_t DownloadCommand::run() {
    getBufferSize();
    if (!getRawInputPath() || !getCompleteInputPath() || !getRealInputPath() || !getOutputPath()) {
        return Error;
    }
    console::out::verbose("input path: " + input_path_real.string() + "\noutput path: " + output_path.string());
    if (!beginHandler()) {
        return Error;
    }
    createInfoThread();
    const bool success{createOutputFile() && startTransmition()};
    deleteFileInstance();
    return success ? Success : Error;
}

void DownloadCommand::getBufferSize() {
    const Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    buffer_size = values.cmd_download_buffer_size;
}

bool DownloadCommand::getRawInputPath() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return false;
    }
    input_path_raw = fs::path{args[0]};
    return true;
}

bool DownloadCommand::getCompleteInputPath() {
    IOGetCompletePathRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_raw);
    if (!handler.run()) {
        return false;
    }
    if (!handler.fileExists()) {
        console::out::err("the target file \"" + input_path_raw.string() + "\" does not exists");
        return false;
    }
    input_path_complete = handler.getValue();
    return true;
}

bool DownloadCommand::getRealInputPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    handler.setInputPath(input_path_complete);
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

void DownloadCommand::createInfoThread() {
    std::thread t{[&] {
        console::out::inf("downloading file...", false);
        while (!transmition_stopped_flag.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::seconds{1});
            printInfoMessage();
        }
        console::out::inf();
        info_thread_stopped_flag.store(true, std::memory_order_release);
    }};
    t.detach();
}

void DownloadCommand::printInfoMessage() {
    static uint64_t last_buffer_index{};
    uint64_t buffer_index{curr_buffer_idx.load(std::memory_order_acquire)};
    uint64_t percentage{static_cast<uint64_t>(static_cast<float>(buffer_index) / buffer_count * 100.0f)};
    uint64_t buffer_per_sec{buffer_index - last_buffer_index};
    uint64_t bytes_per_sec{buffer_per_sec * buffer_size};
    uint64_t remaining_buffers{buffer_count - buffer_index};
    uint64_t remaining_time_sec{buffer_per_sec == 0 ? 0 : remaining_buffers / buffer_per_sec};
    static uint64_t last_remaining_time_sec{};
    uint64_t remaining_time_sec_avg{
        last_remaining_time_sec == 0 ? remaining_time_sec : (last_remaining_time_sec - 1 + remaining_time_sec) / 2};
    std::string str{"downloading file: " + std::to_string(percentage) + "% | " +
                    util::string::byteToAutoUnit(bytes_per_sec) +
                    "/s | eta: " + util::string::secondsToDHMS(remaining_time_sec_avg) + " (" +
                    std::to_string(buffer_index) + "/" + std::to_string(buffer_count) + " buffers)"};
    static std::string last_str{};
    console::out::inf("\r" + util::string::makeFilledString(last_str.size()), false);
    console::out::inf("\r" + str, false);
    last_str = str;
    last_remaining_time_sec = remaining_time_sec;
    last_buffer_index = buffer_index;
}

bool DownloadCommand::startTransmition() {
    const uint64_t buffer_count{handler.getBufferCount()};
    for (; handler.available(); curr_buffer_idx.fetch_add(1, std::memory_order_release)) {
        if (!processNextBuffer()) {
            return false;
        }
    }
    transmition_stopped_flag.store(true, std::memory_order_release);
    while (!info_thread_stopped_flag.load(std::memory_order_acquire))
        ;
    return true;
}

bool DownloadCommand::processNextBuffer() {
    if (!handler.downloadNextBuffer()) {
        return false;
    }
    const std::vector<char>& buffer{handler.getCurrentBuffer()};
    output_file->write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    return true;
}

void DownloadCommand::deleteFileInstance() {
    if (output_file) {
        output_file->close();
        delete output_file;
    }
}
