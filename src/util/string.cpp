#include <cctype>
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

std::string util::string::byteToAutoUnit(uint64_t num) {
    if (num < 0x400) {
        return std::to_string(num) + " b";
    } else if (num < 0x100000) {
        return std::to_string(num / 0x400) + " Kb";
    } else if (num < 0x40000000) {
        return std::to_string(num / 0x100000) + " Mb";
    } else if (num < 0x10000000000) {
        return std::to_string(num / 0x40000000) + " Gb";
    } else if (num < 0x4000000000000) {
        return std::to_string(num / 0x10000000000) + " Tb";
    }
    return std::to_string(num) + " b";
}
