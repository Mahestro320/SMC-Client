#include "shell/commands/help.hpp"

#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/commands_infos.hpp"
#include "util/string.hpp"

exit_code_t HelpCommand::run() {
    if (args.empty()) {
        printAllCommandsInfos();
    } else {
        return printLongDescription();
    }
    return Success;
}

bool HelpCommand::canSee(const CommandInfos& cmd) const {
    const User& user{client->getUser()};
    return user.role >= cmd.min_role;
}

void HelpCommand::printAllCommandsInfos() const {
    printBeginMessage();
    const bool is_client_logged{client->isLogged()};
    for (const CommandInfos& cmd : commands_infos) {
        if ((cmd.must_be_logged && !is_client_logged) || !canSee(cmd)) {
            continue;
        }
        printCommandInfos(cmd);
    }
}

void HelpCommand::printBeginMessage() const {
    console::out::inf("to get more information about an command, type \"help <command name>\"");
    console::out::inf("available commands usage:");
    console::out::inf("| name" + util::string::makeFilledString(NAME_SPACES_COUNT - 4) + " | short name" +
                      util::string::makeFilledString(NAME_SPACES_COUNT - 10) + " | short description" +
                      util::string::makeFilledString(SHORT_DESCRIPTION_SPACES_COUNT - 17) + " | can use offline" +
                      util::string::makeFilledString(CAN_USE_OFFLINE_SPACES_COUNT - 15) + " |\n");
}

void HelpCommand::printCommandInfos(const CommandInfos& cmd) const {
    console::out::inf(
        "  " + cmd.name + util::string::makeFilledString(NAME_SPACES_COUNT - cmd.name.size() + 3) + cmd.short_name +
        util::string::makeFilledString(SHORT_NAME_SPACES_COUNT - cmd.short_name.size() + 3) + cmd.short_description +
        util::string::makeFilledString(SHORT_DESCRIPTION_SPACES_COUNT - cmd.short_description.size() + 3) +
        util::string::boolToYesOrNo(cmd.can_use_offline));
}

exit_code_t HelpCommand::printLongDescription() const {
    for (const CommandInfos& cmd : commands_infos) {
        if (cmd.name == args[0] || cmd.short_name == args[0]) {
            printLongDescriptionOf(cmd);
            return Success;
        }
    }
    console::out::err("error: help: unknown command: " + std::string{args[0]});
    return Error;
}

void HelpCommand::printLongDescriptionOf(const CommandInfos& cmd) const {
    if (canSee(cmd)) {
        console::out::inf((cmd.long_description.empty()) ? cmd.short_description : cmd.long_description);
        return;
    }
    const User& user{client->getUser()};
    console::out::err("you can't use this command as an " + role::getName(user.role));
}
