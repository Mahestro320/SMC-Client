#pragma once

#include <string>

namespace util::string {

extern std::string boolToYesOrNo(bool boolean);
extern void toLowercase(std::string& string);
extern std::string makeFilledString(uint64_t size, char fill_char = ' ');
extern std::string byteToAutoUnit(uint64_t num);
extern std::string secondsToDHMS(uint64_t secs);

} // namespace util::string
