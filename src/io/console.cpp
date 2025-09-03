#include "io/console.hpp"

#include <iomanip>
#include <iostream>
#include "shell/config.hpp"

std::string console::in::getLine() {
    std::string line{};
    std::getline(std::cin, line);
    return line;
}

void console::out::inf(const std::string& msg, bool line_return) {
    std::cout << msg << ((line_return) ? "\n" : "");
}

void console::out::err(const std::string& msg, bool line_return) {
    std::cerr << msg << ((line_return) ? "\n" : "");
}

void console::out::err(const std::exception& e, bool line_return) {
    console::out::err(e.what());
}

void console::out::verbose(const std::string& msg, bool line_return) {
    Config& config{Config::getInstance()};
    ConfigValues config_values{config.getValues()};
    if (config_values.shell_verbose) {
        console::out::inf(msg, line_return);
    }
}
