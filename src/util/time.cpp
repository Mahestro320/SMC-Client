#include "util/time.hpp"

#include <chrono>

namespace {

template<typename duration, std::enable_if_t<std::chrono::_Is_duration_v<duration>, bool> = true>
uint64_t getTimestamp() {
    const auto now{std::chrono::system_clock::now()};
    const auto time{std::chrono::duration_cast<duration>(now.time_since_epoch())};
    return time.count();
}

} // namespace

uint64_t util::time::getSeconds() {
    return getTimestamp<std::chrono::seconds>();
}

uint64_t util::time::getMillis() {
    return getTimestamp<std::chrono::milliseconds>();
}

uint64_t util::time::getNanos() {
    return getTimestamp<std::chrono::nanoseconds>();
}
