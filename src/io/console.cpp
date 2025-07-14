#include <iostream>
#include "io/console.hpp"
#include "shell/config.hpp"

std::string console::in::getLine() {
    std::string line{};
    std::getline(std::cin, line);
    return line;
}

void console::out::ptr(const void* ptr, bool line_return) {
    std::cout << std::hex << std::setw(sizeof(decltype(ptr))) << std::setfill('0') << ptr << std::dec;
    if (line_return) {
        std::cout << std::endl;
    }
}

void console::out::inf(const std::string& msg, bool line_return) {
    std::cout << msg;
    if (line_return) {
        std::cout << std::endl;
    }
}

void console::out::err(const std::string& msg, bool line_return) {
    std::cout << msg;
    if (line_return) {
        std::cout << std::endl;
    }
}

void console::out::err(const std::exception& e, bool line_return) {
    std::cout << e.what();
    if (line_return) {
        std::cout << std::endl;
    }
}

void console::out::verbose(const std::string& msg, bool line_return) {
    Config& config{Config::getInstance()};
    ConfigValues config_values{config.getValues()};
    if (config_values.shell_verbose) {
        inf(msg, line_return);
    }
}
