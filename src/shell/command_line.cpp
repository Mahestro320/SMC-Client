#include "io/console.hpp"
#include "shell/command_line.hpp"

void CommandLine::getLine() {
    raw_line = console::in::getLine();
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
