#include "shell/commands/upload.hpp"

#include <boost/asio.hpp>

#include "io/console.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_file_exists.hpp"
#include "shell/config.hpp"
#include "shell/system/cursor.hpp"
#include "util/error_message.hpp"
#include "util/fs.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

exit_code_t UploadCommand::run() {
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

bool UploadCommand::init() {
    getInputPath();
    return checkInputPath() && loadFiles();
}

void UploadCommand::getInputPath() {
    input_path = fs::path{args[0]};
}

bool UploadCommand::checkInputPath() const {
    if (!fs::exists(input_path)) {
        console::out::err("the file does not exists");
        return false;
    }
    return true;
}

bool UploadCommand::loadFiles() {
    path_parent = input_path.parent_path();
    if (fs::is_regular_file(input_path)) {
        input_files.push_back(input_path);
    } else if (fs::is_directory(input_path)) {
        return util::fs::pushDirContentIn(input_files, input_path);
    } else {
        console::out::err("error: unhandled file type");
        return false;
    }
    return true;
}

bool UploadCommand::startTransmition() {
    loadBufferSize();
    console::out::inf(std::string{(input_files.size() <= 1) ? "" : "\n"} + "press ESC to cancel\n");
    for (const fs::path& filepath : input_files) {
        if (stop_flag.load(std::memory_order_acquire)) {
            break;
        }
        if (!uploadFile(filepath)) {
            return false;
        }
    }
    transmition_finished_flag.store(true, std::memory_order_relaxed);
    return true;
}

bool UploadCommand::uploadFile(const std::filesystem::path& path) {
    if (input_files.size() > 1) {
        console::out::inf("uploading file \"" + path.string() + '"');
    }
    IOSetFileContentRH handler{std::move(getNewIOSetFileContentRH(path))};
    std::ifstream* file{};
    if (!getInputFile(path, file)) {
        return false;
    }
    handler.setFileSize(file_size);
    if (!handler.run()) {
        return false;
    }
    buffer_count.store(handler.getBufferCount(), std::memory_order_release);
    createInfoThread();
    for (; handler.available(); curr_buffer_idx.fetch_add(1, std::memory_order_release)) {
        std::vector<char>& buffer{handler.getCurrentBuffer()};
        file->read(buffer.data(), static_cast<std::streamsize>(buffer_size));
        if (stop_flag.load(std::memory_order_acquire) || !handler.uploadNextBuffer()) {
            handler.stop();
            util::fs::freeStream(file);
            return false;
        }
    }
    curr_buffer_idx.store(0, std::memory_order_release);
    util::fs::freeStream(file);
    stopInfoThread();
    return true;
}

void UploadCommand::loadBufferSize() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    buffer_size = config_values.cmd_upload_buffer_size;
}

IOSetFileContentRH UploadCommand::getNewIOSetFileContentRH(const fs::path& path) {
    IOSetFileContentRH handler{};
    handler.setBufferSize(static_cast<size_t>(buffer_size));
    handler.setClient(client);
    handler.setPath(getTargetPathFromSource(path));
    return handler;
}

fs::path UploadCommand::getTargetPathFromSource(const fs::path& path) const {
    fs::path output{};
    size_t i{};
    const User& user{client->getUser()};
    const fs::path norm_path{user.current_dir / fs::relative(path, path_parent)};
    const fs::path norm_path_lex_normal{norm_path.lexically_normal()};
    const fs::path parent_path{norm_path_lex_normal.parent_path()};
    const fs::path filename_no_ext{norm_path_lex_normal.stem()};
    const fs::path ext{norm_path_lex_normal.extension()};
    IOFileExistsRH handler{};
    handler.setClient(client);
    do {
        output = parent_path /
                 fs::path{filename_no_ext.string() + ((i == 0) ? "" : '_' + std::to_string(i + 1)) + ext.string()};
        ++i;
        handler.setPath(output);
        handler.run();
    } while (handler.getValue());
    return output;
}

bool UploadCommand::getInputFile(const fs::path& path, std::ifstream*& stream) {
    stream = new std::ifstream{path, std::ios::binary | std::ios::ate};
    if (!stream->is_open()) {
        delete stream;
        console::out::err("error while opening file \"" + path.string() + "\": " + util::error_message::get(errno));
        return false;
    }
    file_size = static_cast<uint64_t>(stream->tellg());
    stream->seekg(0, std::ios::beg);
    return true;
}
