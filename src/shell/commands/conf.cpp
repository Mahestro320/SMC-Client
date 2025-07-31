#include "io/console.hpp"
#include "shell/commands/conf.hpp"
#include "shell/config.hpp"

exit_code_t ConfCommand::run() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return Error;
    }
    return runOption(args[0]) ? Success : Error;
}

bool ConfCommand::runOption(const std::string& option) {
    if (option == "load") {
        return runLoadOption();
    } else if (option == "save") {
        return runSaveOption();
    } else if (option == "print") {
        return runPrintOption();
    } else if (option == "set") {
        return runSetOption();
    } else if (option == "get") {
        return runGetOption();
    }
    console::out::err("unknown option: " + option);
    return false;
}

bool ConfCommand::runLoadOption() const {
    Config& config{Config::getInstance()};
    return config.load();
}

bool ConfCommand::runSetOption() const {
    if (args.size() < 3) {
        console::out::err("invalid command argument");
        return false;
    }
    Config& config{Config::getInstance()};
    return config.setValue(args[1], args[2]);
}

bool ConfCommand::runSaveOption() const {
    Config& config{Config::getInstance()};
    return config.updateFile();
}

bool ConfCommand::runGetOption() const {
    if (args.size() < 2) {
        console::out::err("invalid command argument");
        return false;
    }
    const std::string& key{args[1]};
    Config& config{Config::getInstance()};
    const std::string value{config.getValue(key)}, resolved_value{config.getResolvedValue(key)};
    console::out::inf(key + ": " + value);
    if (value != resolved_value) {
        console::out::inf(key + " (resolved): " + resolved_value);
    }
    return true;
}

bool ConfCommand::runPrintOption() const {
    Config& config{Config::getInstance()};
    bool first{true};
    for (const auto& section : config.getPropertyTree()) {
        if (!first) {
            console::out::inf();
        }
        first = false;
        console::out::inf("[" + section.first + "]");
        for (const auto& kv : section.second) {
            console::out::inf(kv.first + "=" + kv.second.get_value<std::string>());
        }
    }
    return true;
}
