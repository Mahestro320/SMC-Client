#include "network/request/handlers/io_get_complete_path.hpp"

#include "io/console.hpp"
#include "network/client.hpp"

bool IOGetCompletePathRH::run() {
    handler.setClient(client);
    if (checkIfPathIsRelative()) {
        return true;
    } else if (error) {
        return false;
    } else if (!checkIfPathIsAbsolute()) {
        return false;
    }
    return true;
}

bool IOGetCompletePathRH::checkIfPathIsRelative() {
    if (!isPathRelative()) {
        return false;
    }
    does_file_exists = true;
    output_path = input_path_relative;
    console::out::verbose("complete path: " + output_path.string());
    return true;
}

bool IOGetCompletePathRH::checkIfPathIsAbsolute() {
    if (!isPathAbsolute()) {
        if (!error) {
            console::out::err("the path does not exists");
        }
        return false;
    }
    does_file_exists = true;
    output_path = input_path_absolute;
    console::out::verbose("complete path: " + output_path.string());
    return true;
}

bool IOGetCompletePathRH::isPathRelative() {
    handler.setPath(input_path_relative);
    if (!handler.run()) {
        error = true;
        return false;
    }
    if (handler.getValue()) {
        console::out::verbose("the path is relative");
        return true;
    }
    return false;
}

bool IOGetCompletePathRH::isPathAbsolute() {
    handler.setPath(input_path_absolute);
    if (!handler.run()) {
        error = true;
        return false;
    }
    if (handler.getValue()) {
        console::out::verbose("the path is absolute");
        return true;
    }
    return false;
}

void IOGetCompletePathRH::setPath(const std::filesystem::path& path) {
    const User& user{client->getUser()};
    input_path_relative = user.current_dir / path;
    input_path_absolute = path;
}

bool IOGetCompletePathRH::fileExists() const {
    return does_file_exists;
}

const std::filesystem::path& IOGetCompletePathRH::getValue() const {
    return output_path;
}
