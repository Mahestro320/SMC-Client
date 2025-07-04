#include "io/console.hpp"
#include "shell/commands/conf.hpp"
#include "shell/config.hpp"

exit_code_t ConfCommand::run() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return Error;
    }
    const std::string& option{args[0]};
    if (option == "load") {
        return runLoadOption();
    } else if (option == "updatefile") {
        return runUpdateFileOption();
    } else if (option == "printvals") {
        return runPrintValsOption();
    } else if (option == "set") {
        return runSetOption();
    } else if (option == "get") {
        return runGetOption();
    } else {
        console::out::err("unknown option: " + option);
        return Error;
    }
    return Success;
}

exit_code_t ConfCommand::runLoadOption() const {
    Config& config{Config::getInstance()};
    return config.load() ? Success : Error;
}

exit_code_t ConfCommand::runSetOption() const {
    if (args.size() < 3) {
        console::out::err("invalid command argument");
        return Error;
    }
    const std::string& key{args[1]};
    const std::string& value{args[2]};
    console::out::verbose("setting \"" + key + "\" to \"" + value + "\"");
    Config& config{Config::getInstance()};
    return config.setValue(key, value) ? Success : Error;
}

exit_code_t ConfCommand::runUpdateFileOption() const {
    Config& config{Config::getInstance()};
    console::out::inf("updating config file");
    return config.updateFile() ? Success : Error;
}

exit_code_t ConfCommand::runGetOption() const {
    if (args.size() < 2) {
        console::out::err("invalid command arguments");
        return Error;
    }
    const std::string& key{args[1]};
    Config& config{Config::getInstance()};
    console::out::inf(key + ": " + config.getValue(key));
    return Success;
}

exit_code_t ConfCommand::runPrintValsOption() const {
    Config& config{Config::getInstance()};
    for (const auto& section : config.getPropertyTree()) {
        console::out::inf("[" + section.first + "]");
        for (const auto& kv : section.second) {
            console::out::inf(kv.first + " = " + kv.second.get_value<std::string>());
        }
        console::out::inf();
    }
    return Success;
}
