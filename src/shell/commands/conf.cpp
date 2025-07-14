#include "io/console.hpp"
#include "shell/commands/conf.hpp"
#include "shell/config.hpp"

exit_code_t ConfCommand::run() {
    if (args.empty()) {
        console::out::err("invalid command argument");
        return Error;
    }
    const std::string& option{};
    return runOption(args[0]) ? Success : Error;
}

bool ConfCommand::runOption(const std::string& name) {
    if (name == "load") {
        return runLoadOption();
    } else if (name == "updatefile") {
        return runUpdateFileOption();
    } else if (name == "printvals") {
        return runPrintValsOption();
    } else if (name == "set") {
        return runSetOption();
    } else if (name == "get") {
        return runGetOption();
    } else {
        console::out::err("unknown option: " + name);
        return false;
    }
    return true;
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
    const std::string& key{args[1]};
    const std::string& value{args[2]};
    console::out::verbose("setting \"" + key + "\" to \"" + value + "\"");
    Config& config{Config::getInstance()};
    return config.setValue(key, value);
}

bool ConfCommand::runUpdateFileOption() const {
    Config& config{Config::getInstance()};
    console::out::inf("updating config file");
    return config.updateFile();
}

bool ConfCommand::runGetOption() const {
    if (args.size() < 2) {
        console::out::err("invalid command arguments");
        return false;
    }
    const std::string& key{args[1]};
    Config& config{Config::getInstance()};
    console::out::inf(key + ": " + config.getValue(key));
    console::out::inf(key + " (resolved): " + config.getResolvedValue(key));
    return true;
}

bool ConfCommand::runPrintValsOption() const {
    Config& config{Config::getInstance()};
    for (const auto& section : config.getPropertyTree()) {
        console::out::inf("[" + section.first + "]");
        for (const auto& kv : section.second) {
            console::out::inf(kv.first + " = " + kv.second.get_value<std::string>());
        }
        console::out::inf();
    }
    return true;
}
