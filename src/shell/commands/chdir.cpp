#include "io/console.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/commands/chdir.hpp"
#include "user.hpp"

namespace fs = std::filesystem;

exit_code_t ChdirCommand::run() {
    if (!getInputPath() || !isInputPathDirectory()) {
        return Error;
    }
    setNewCurrentDir();
    return Success;
}

bool ChdirCommand::getInputPath() {
    return getRawInputPath() && readCompleteInputPath() && readRealInputPath();
}

bool ChdirCommand::getRawInputPath() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return false;
    }
    target_path_raw = fs::path{args[0]};
    return true;
}

bool ChdirCommand::readCompleteInputPath() {
    IOGetCompletePathRH handler{};
    handler.setClient(client);
    handler.setPath(target_path_raw);
    if (!handler.run() || !handler.fileExists()) {
        return false;
    }
    target_path_complete = handler.getValue();
    return true;
}

bool ChdirCommand::readRealInputPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    handler.setPath(target_path_complete);
    if (!handler.run()) {
        return false;
    }
    target_path_real = handler.getValue();
    return true;
}

bool ChdirCommand::isInputPathDirectory() const {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(target_path_real);
    if (!handler.run()) {
        return false;
    }
    if (handler.getValue() != FileType::Directory) {
        console::out::err("the given path isn't a directory");
        return false;
    }
    console::out::verbose("the path is a directory");
    return true;
}

void ChdirCommand::setNewCurrentDir() {
    User& user{client->getUser()};
    user.current_dir = target_path_real.lexically_normal();
    const std::string current_dir_str{user.current_dir.generic_string()};
    if (current_dir_str.size() == 2 && std::isalpha(current_dir_str[0]) && current_dir_str[1] == ':') {
        user.current_dir = fs::path{current_dir_str + "\\"};
        user.current_dir = user.current_dir.lexically_normal();
    }
}
