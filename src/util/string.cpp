#include <format>
#include "util/string.hpp"

std::string util::string::boolToYesOrNo(bool boolean) {
    return boolean ? "yes" : "no";
}

void util::string::toLowercase(std::string& string) {
    for (char& c : string) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

std::string util::string::makeFilledString(uint64_t size, char fill_char) {
    std::string result{};
    for (uint32_t i{0}; i < size; ++i) {
        result += fill_char;
    }
    return result;
}

std::string util::string::secondsToDHMS(uint64_t secs) {
    const uint64_t days{secs / 86400};
    secs %= 86400;
    const uint64_t hours{secs / 3600};
    secs %= 3600;
    const uint64_t minutes{secs / 60};
    const uint64_t seconds{secs % 60};
    return std::format("{}:{:02}:{:02}:{:02}", days, hours, minutes, seconds);
}

std::string util::string::formatFloat(float value, uint64_t precision) {
    return std::format("{:.{}f}", value, precision);
}
