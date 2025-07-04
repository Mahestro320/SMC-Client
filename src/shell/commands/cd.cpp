#include "io/console.hpp"
#include "network/request/handlers/io_file_exists.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/commands/cd.hpp"

namespace fs = std::filesystem;

exit_code_t CdCommand::run() {
    if (!getTargetPath() || !fileExists() || !isFileDirectory()) {
        return Error;
    }
    setNewUserCurrentDir();
    return setUserCurrentDirRealPath() ? Success : Error;
}

bool CdCommand::getTargetPath() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return false;
    }
    const User& user{client->getUser()};
    target_path_relative = user.current_dir / fs::path{args[0]};
    target_path_absolute = fs::path{args[0]};
    return true;
}

bool CdCommand::fileExists() {
    IOFileExistsRH handler{};
    handler.setClient(client);
    if (relativePathExists(handler)) {
        console::out::verbose("the given path is relative");
        target_path_valid = target_path_relative;
        return true;
    }
    if (absolutePathExists(handler)) {
        console::out::verbose("the given path is absolute");
        target_path_valid = target_path_absolute;
        console::out::verbose("the folder exists");
        return true;
    }
    console::out::err("the folder does not exists");
    return false;
}

bool CdCommand::relativePathExists(IOFileExistsRH& handler) const {
    handler.setPath(target_path_relative);
    return handler.run() && handler.doesFileExists();
}

bool CdCommand::absolutePathExists(IOFileExistsRH& handler) const {
    handler.setPath(target_path_absolute);
    return handler.run() && handler.doesFileExists();
}

bool CdCommand::isFileDirectory() const {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(target_path_valid);
    if (!handler.run()) {
        return false;
    }
    if (handler.fileType() != FileType::Directory) {
        console::out::err("the given path isn't a directory");
        return false;
    }
    console::out::verbose("the path is a directory !");
    return true;
}

void CdCommand::setNewUserCurrentDir() {
    User& user{client->getUser()};
    user.current_dir = target_path_valid.lexically_normal();
    const std::string current_dir_str{user.current_dir.generic_string()};
    if (current_dir_str.size() == 2 && std::isalpha(current_dir_str[0]) && current_dir_str[1] == ':') {
        user.current_dir = fs::path{current_dir_str + "\\"};
        user.current_dir = user.current_dir.lexically_normal();
    }
}

bool CdCommand::setUserCurrentDirRealPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    User& user{client->getUser()};
    handler.setInputPath(user.current_dir);
    if (!handler.run()) {
        return false;
    }
    user.current_dir = handler.realPath();
    return true;
}
