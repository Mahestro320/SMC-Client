#pragma once

#include <string>
#include <vector>
#include "user/role.hpp"

struct CommandInfos {
    std::string name{};
    std::string short_description{};
    std::string long_description{};
    bool can_use_offline{};
    bool must_be_logged{};
    Role min_role{Role::User};
};

static inline const std::vector<CommandInfos> commands_infos{
    CommandInfos{.name = "help", .short_description = "show help", .can_use_offline = true},
    CommandInfos{.name = "cls", .short_description = "clear the screen", .can_use_offline = true},
    CommandInfos{.name = "quit", .short_description = "exit the shell", .can_use_offline = true},
    CommandInfos{
        .name = "conf",
        .long_description = "usage: conf <option> [option args]\nthe options are:\n"
                            "\tload        load file\n"
                            "\tupdatefile  update file with new values\n"
                            "\tprintvals   prints all the values\n"
                            "\tset         set a value with a key using [<key> <value>]\n"
                            "\tget         gets a value from a key using [<key>]",
        .can_use_offline = true,
    },
    CommandInfos{
        .name = "login",
        .short_description = "log in as an user",
        .long_description = "log in as an user\nusage: login <username> <password>",
    },
    CommandInfos{.name = "logout", .short_description = "log out", .must_be_logged = true},
    CommandInfos{
        .name = "server",
        .long_description = "usage: server <option>\nthe options are:\n\tconnect    connects to the server",
    },
    CommandInfos{
        .name = "dir",
        .short_description = "show the content of the current directory",
        .must_be_logged = true,
    },
    CommandInfos{
        .name = "cd",
        .short_description = "sets the current directory",
        .long_description =
            "usage: cd <location>\nsets the current directory at the location relative to the current directory",
        .must_be_logged = true,
    },
};
