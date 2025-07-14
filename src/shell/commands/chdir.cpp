#include "io/console.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/commands/chdir.hpp"
#include "user.hpp"

namespace fs = std::filesystem;

exit_code_t ChdirCommand::run() {
    if (!getTargetPath() || !getCompletePath() || !isFileDirectory()) {
        return Error;
    }
    setNewUserCurrentDir();
    return setUserCurrentDirRealPath() ? Success : Error;
}

bool ChdirCommand::getTargetPath() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return false;
    }
    target_path = fs::path{args[0]};
    return true;
}

bool ChdirCommand::getCompletePath() {
    IOGetCompletePathRH handler{};
    handler.setClient(client);
    handler.setPath(target_path);
    if (!handler.run() || !handler.fileExists()) {
        return false;
    }
    target_path_valid = handler.getValue();
    return true;
}

bool ChdirCommand::isFileDirectory() const {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(target_path_valid);
    if (!handler.run()) {
        return false;
    }
    if (handler.getValue() != FileType::Directory) {
        console::out::err("the given path isn't a directory");
        return false;
    }
    console::out::verbose("the path is a directory !");
    return true;
}

void ChdirCommand::setNewUserCurrentDir() {
    User& user{client->getUser()};
    user.current_dir = target_path_valid.lexically_normal();
    const std::string current_dir_str{user.current_dir.generic_string()};
    if (current_dir_str.size() == 2 && std::isalpha(current_dir_str[0]) && current_dir_str[1] == ':') {
        user.current_dir = fs::path{current_dir_str + "\\"};
        user.current_dir = user.current_dir.lexically_normal();
    }
}

bool ChdirCommand::setUserCurrentDirRealPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    User& user{client->getUser()};
    handler.setInputPath(user.current_dir);
    if (!handler.run()) {
        return false;
    }
    user.current_dir = handler.getValue();
    return true;
}
