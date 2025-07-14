#include "io/console.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_complete_path.hpp"

bool IOGetCompletePathRH::run() {
    handler.setClient(client);
    if (isPathRelative()) {
        does_file_exists = true;
        output_path = input_path_relative;
        return true;
    }
    if (error) {
        return false;
    }
    if (!isPathAbsolute()) {
        if (!error) {
            console::out::inf("the path does not exists");
        }
        return error;
    }
    does_file_exists = true;
    output_path = input_path_absolute;
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
