#include "shell/commands/download.hpp"

#include "io/console.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "network/request/handlers/util/get_dir_content_recursively.hpp"
#include "shell/config.hpp"
#include "util/error_message.hpp"
#include "util/fs.hpp"

namespace fs = std::filesystem;

DownloadCommand::~DownloadCommand() {
    delete info_thread;
}

exit_code_t DownloadCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    if (!init()) {
        return Error;
    }
    try {
        _run();
    } catch (const std::exception& e) {
        console::out::err("error: " + std::string{e.what()});
        stop_flag.store(true, std::memory_order_release);
        stopInfoThread();
        return Error;
    }
    return Success;
}

bool DownloadCommand::init() {
    if (!getInputPath() || !getInputFiles()) {
        return false;
    }
    return true;
}

bool DownloadCommand::getInputPath() {
    getInputPathRaw();
    return getInputPathComplete() && getInputPathReal();
}

void DownloadCommand::getInputPathRaw() {
    input_path_raw = args[0];
}

bool DownloadCommand::getInputPathComplete() {
    IOGetCompletePathRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_raw);
    if (!handler.run()) {
        return false;
    }
    if (!handler.fileExists()) {
        console::out::err("the path does not exists");
        return false;
    }
    input_path_complete = handler.getValue();
    return true;
}

bool DownloadCommand::getInputPathReal() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_complete);
    if (!handler.run()) {
        return false;
    }
    input_path_real = handler.getValue();
    return true;
}

bool DownloadCommand::getInputFiles() {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(input_path_real);
    if (!handler.run()) {
        return false;
    }
    const FileType file_type{handler.getValue()};
    if (file_type == FileType::File) {
        input_files.push_back(input_path_real);
    } else if (file_type == FileType::Directory) {
        console::out::inf("getting files to download");
        input_files = getDirectoryContent();
        console::out::inf("found " + std::to_string(input_files.size()) + " files");
    } else {
        console::out::err("error: unknown file type");
        return false;
    }
    return true;
}

std::vector<fs::path> DownloadCommand::getDirectoryContent() const {
    GetDirContentRecursively handler{};
    handler.setClient(client);
    handler.setPath(input_path_real);
    if (!handler.run()) {
        return {};
    }
    return handler.getContent();
}

bool DownloadCommand::startTransmition() {
    console::out::inf(std::string{(input_files.size() <= 1) ? "" : "\n"} + "press ESC to cancel\n");
    for (const fs::path& filepath : input_files) {
        if (stop_flag.load(std::memory_order_acquire)) {
            break;
        }
        if (!downloadFile(filepath)) {
            return false;
        }
    }
    transmition_finished_flag.store(true, std::memory_order_relaxed);
    return true;
}

bool DownloadCommand::downloadFile(const fs::path& path) {
    if (input_files.size() > 1) {
        console::out::inf("downloading file \"" + path.string() + '"');
    }
    IOGetFileContentRH handler{std::move(getNewIOGetFileContentRH(path))};
    std::ofstream* file{};
    if (!handler.run() || !createOutputFile(path, file)) {
        return false;
    }
    buffer_count.store(handler.getBufferCount(), std::memory_order_release);
    createInfoThread();
    for (; handler.available(); curr_buffer_idx.fetch_add(1, std::memory_order_release)) {
        if (stop_flag.load(std::memory_order_acquire) || !handler.downloadNextBuffer()) {
            handler.stop();
            util::fs::freeStream(file);
            return false;
        }
        util::fs::writeBufferInFile(*file, handler.getCurrentBuffer());
    }
    curr_buffer_idx.store(0, std::memory_order_release);
    util::fs::freeStream(file);
    stopInfoThread();
    return true;
}

IOGetFileContentRH DownloadCommand::getNewIOGetFileContentRH(const fs::path& path) {
    IOGetFileContentRH handler{};
    handler.setClient(client);
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    handler.setBufferSize(config_values.cmd_download_buffer_size);
    handler.setPath(path);
    return handler;
}

bool DownloadCommand::createOutputFile(const fs::path& path, std::ofstream*& file) const {
    const fs::path output_path{util::fs::getNonExistingPath(getOutputPathFromSource(path))};
    const fs::path output_path_parent{output_path.parent_path()};
    fs::create_directories(output_path_parent);
    file = new std::ofstream{output_path, std::ios::binary | std::ios::beg};
    if (!file->is_open()) {
        delete file;
        console::out::err("error while creating file \"" + path.string() + "\": " + util::error_message::get(errno));
        return false;
    }
    return true;
}

fs::path DownloadCommand::getOutputPathFromSource(const fs::path& source) const {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    const fs::path relative_path{fs::relative(source, input_path_real.parent_path())};
    return fs::path{config_values.cmd_download_output} / relative_path;
}
