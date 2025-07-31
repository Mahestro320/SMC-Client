#pragma once

#include <deque>
#include "shell/util/textual_loading_bar.hpp"

class DownloadInfoPrinter final {
  private:
    static inline constexpr uint64_t MAX_REMAINING_TIMES_QUEUE_SIZE{5};
    static inline constexpr uint16_t LOADING_BAR_SIZE{40};

    uint64_t buffer_size{};
    uint64_t buffer_count{};

    TextualLoadingBar loading_bar{LOADING_BAR_SIZE};
    std::string first_text{}, second_text{};

    bool first_print{true};
    uint64_t last_first_text_size{};
    uint64_t last_buffer_idx{};
    uint64_t curr_buffer_idx{};
    float percentage{};
    uint64_t bytes_sent_per_sec{};

    uint64_t remaining_time_sec{};
    uint64_t remaining_time_sec_avg{};
    std::deque<uint64_t> remaining_times{};

    void updateCurrValues();
    void updateLastValues();
    void updateRemainingTimeAvg();
    void updateText();

    void printFirstTextSpaceFill();

    uint64_t getBytesSentPerSec() const;
    uint64_t getRemainingBytes() const;

  public:
    DownloadInfoPrinter() = default;

    void setBufferSize(uint64_t buffer_size);
    void setBufferCount(uint64_t buffer_count);
    void setCurrBufferIndex(uint64_t curr_buffer_idx);
    void update();
    void print();
};
