#include "io/console.hpp"
#include "io/file_info.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"
#include "shell/commands/dir.hpp"
#include "util/byte.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;

exit_code_t DirCommand::run() {
    tcp::socket& socket{client->getSocket()};
    IOGetDirContentRH handler{};
    handler.setClient(client);
    const User& user{client->getUser()};
    handler.setPath(user.current_dir);
    if (!handler.run()) {
        return Error;
    }
    data = handler.getData();
    printData();
    return Success;
}

void DirCommand::printData() const {
    const User& user{client->getUser()};
    console::out::verbose();
    console::out::inf("content of \"" + user.current_dir.string() + "\" (" + std::to_string(data.size()) + " files):");
    console::out::inf(" | type " + util::string::makeFilledString(TYPE_SPACES_COUNT - 9) + " | size " +
                      util::string::makeFilledString(TYPE_SPACES_COUNT - 10) + " | name\n");
    if (data.empty()) {
        console::out::inf("\t(empty)");
        return;
    }
    for (const FileInfo& file : data) {
        const std::string type_str{file_type::getName(file.type)};
        const std::string size_str{file.type == FileType::File ? util::byte::toAutoUnit(file.size) : ""};
        console::out::inf("\t" + type_str + util::string::makeFilledString(TYPE_SPACES_COUNT - type_str.size()) +
                          size_str + util::string::makeFilledString(SIZE_SPACES_COUNT - size_str.size()) + file.name);
    }
}
