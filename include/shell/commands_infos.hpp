#pragma once

#include <string>
#include <vector>
#include "user/role.hpp"

struct CommandInfos {
    std::string name{};
    std::string short_name{};
    std::string short_description{};
    std::string long_description{};
    bool can_use_offline{};
    bool must_be_logged{};
    Role min_role{Role::User};
};

static inline const std::vector<CommandInfos> commands_infos{
    CommandInfos{
        .name = "help",
        .short_description = "show help",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "infos",
        .short_name = "inf",
        .short_description = "print shell infos",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "clear",
        .short_name = "clr",
        .short_description = "clear the screen",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "quit",
        .short_description = "exit the shell",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "conf",
        .long_description = "usage: conf <option> [option args]\nthe options are:\n"
                            "\tload        load file\n"
                            "\tupdatefile  update file with new values\n"
                            "\tprint       prints all the values\n"
                            "\tset         set a value with a key using [<key> <value>]\n"
                            "\tget         gets a value from a key using [<key>]",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "login",
        .short_name = "lgi",
        .short_description = "log in as an user",
        .long_description = "log in as an user\nusage: login <username> <password>",
    },
    CommandInfos{
        .name = "logout",
        .short_name = "lgo",
        .short_description = "log out",
        .must_be_logged = true,
    },
    CommandInfos{
        .name = "client",
        .short_name = "clt",
        .long_description = "usage: client <option>\nthe options are:\n"
                            "\tconnect     connects to the server\n"
                            "\tdisconnect  disconnects from the server\n",
    },
    CommandInfos{
        .name = "dir",
        .short_description = "show the content of the current directory",
        .must_be_logged = true,
    },
    CommandInfos{
        .name = "chdir",
        .short_name = "cd",
        .short_description = "sets the current directory",
        .long_description =
            "usage: cd <location>\nsets the current directory at the location relative to the current directory",
        .must_be_logged = true,
    },
    CommandInfos{
        .name = "download",
        .short_name = "dwl",
        .short_description = "download a file",
        .long_description = "usage: download <input path> <output path (optional)> <s=<buffer size> (optional)>",
        .must_be_logged = true,
    },
};
