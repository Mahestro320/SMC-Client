#include "util/version.hpp"

std::string Version::toString(char delim) const {
    return std::to_string(MAJOR) + delim + std::to_string(MINOR) + delim + std::to_string(PATCH);
}
