#include "io/console.hpp"
#include "shell/command_line.hpp"
#include "util/env_var_str_resolver.hpp"

bool CommandLine::getLine() {
    std::string line{console::in::getLine()};
    EnvVarStrResolver resolver{};
    resolver.setInput(line);
    if (!resolver.resolve()) {
        return false;
    }
    raw_line = resolver.getOutput();
    return true;
}

void CommandLine::tokenize() {
    std::string current_item{};
    bool is_in_quotes{};
    char last{};
    for (char c : raw_line) {
        if (c == '"') {
            is_in_quotes = !is_in_quotes;
            if (last == '"') {
                tokenized_line.push_back("");
            } else {
                flushToken(current_item);
            }
            last = c;
            continue;
        }

        if (is_in_quotes || !std::isspace(static_cast<unsigned char>(c))) {
            current_item += c;
        } else {
            flushToken(current_item);
        }
        last = c;
    }
    flushToken(current_item);
}

void CommandLine::flushToken(std::string& item) {
    if (!item.empty()) {
        tokenized_line.push_back(item);
        item.clear();
    }
}

bool CommandLine::empty() const {
    return raw_line.empty();
}

const std::vector<std::string>& CommandLine::getTokens() const {
    return tokenized_line;
}
