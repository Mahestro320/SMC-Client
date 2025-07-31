#include "io/console.hpp"
#include "shell/commands/download/info_printer.hpp"
#include "shell/system/cursor.hpp"
#include "util/byte.hpp"
#include "util/string.hpp"

void DownloadInfoPrinter::setBufferSize(uint64_t buffer_size) {
    this->buffer_size = buffer_size;
}

void DownloadInfoPrinter::setBufferCount(uint64_t buffer_count) {
    this->buffer_count = buffer_count;
}

void DownloadInfoPrinter::setCurrBufferIndex(uint64_t curr_buffer_idx) {
    this->curr_buffer_idx = curr_buffer_idx;
}

void DownloadInfoPrinter::update() {
    updateCurrValues();
    updateLastValues();
    updateRemainingTimeAvg();
    updateText();
}

void DownloadInfoPrinter::updateCurrValues() {
    percentage = static_cast<float>(curr_buffer_idx) / buffer_count * 100.0f;
    bytes_sent_per_sec = getBytesSentPerSec();
    remaining_time_sec = bytes_sent_per_sec == 0 ? 0 : getRemainingBytes() / bytes_sent_per_sec;
    loading_bar.setValue(static_cast<uint16_t>(static_cast<float>(curr_buffer_idx) / buffer_count * LOADING_BAR_SIZE));
}

uint64_t DownloadInfoPrinter::getBytesSentPerSec() const {
    return (curr_buffer_idx - last_buffer_idx) * buffer_size;
}

uint64_t DownloadInfoPrinter::getRemainingBytes() const {
    return (buffer_count - curr_buffer_idx) * buffer_size;
}

void DownloadInfoPrinter::updateLastValues() {
    last_buffer_idx = curr_buffer_idx;
}

void DownloadInfoPrinter::updateRemainingTimeAvg() {
    remaining_times.push_front(remaining_time_sec);
    if (remaining_times.size() > MAX_REMAINING_TIMES_QUEUE_SIZE) {
        remaining_times.pop_back();
    }
    double total_weight{0.0}, weighted_sum{0.0};
    const uint64_t remaining_times_size{remaining_times.size()};
    for (size_t i{}; i < remaining_times_size; ++i) {
        double weight{static_cast<double>(remaining_times_size - i)};
        weighted_sum += remaining_times[i] * weight;
        total_weight += weight;
    }
    remaining_time_sec_avg = static_cast<uint64_t>(weighted_sum / total_weight);
}

void DownloadInfoPrinter::updateText() {
    first_text = util::byte::toAutoUnit<float>(bytes_sent_per_sec) +
                 "/s, eta: " + util::string::secondsToDHMS(remaining_time_sec_avg);
    second_text = loading_bar.getText() + ' ' + util::string::formatFloat(percentage) + '%';
}

void DownloadInfoPrinter::print() {
    if (first_print) {
        console::out::inf("\n");
        first_print = false;
    }
    shell::cursor::moveUp();
    shell::cursor::moveUp();
    printFirstTextSpaceFill();
    console::out::inf(first_text);
    console::out::inf(second_text + '\r');
    last_first_text_size = first_text.size();
}

void DownloadInfoPrinter::printFirstTextSpaceFill() {
    if (first_text.size() < last_first_text_size) {
        console::out::inf(util::string::makeFilledString(last_first_text_size) + '\r', false);
    }
}
