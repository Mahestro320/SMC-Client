#pragma once

#include <string>

namespace console {

namespace in {

extern std::string getLine();

}

namespace out {

extern void ptr(const void* ptr, bool line_return = true);
extern void inf(const std::string& msg = "", bool line_return = true);
extern void err(const std::string& msg = "", bool line_return = true);
extern void err(const std::exception& e, bool line_return = true);
extern void verbose(const std::string& msg = "", bool line_return = true);

} // namespace out

} // namespace console
