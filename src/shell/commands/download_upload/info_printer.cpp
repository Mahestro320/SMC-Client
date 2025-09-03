#include "shell/commands/download_upload/info_printer.hpp"

#include <cmath>
#include "io/console.hpp"
#include "shell/system/cursor.hpp"
#include "util/byte.hpp"
#include "util/string.hpp"

void DownloadUploadInfoPrinter::setBufferSize(uint64_t buffer_size) {
    this->buffer_size = buffer_size;
}

void DownloadUploadInfoPrinter::setBufferCount(uint64_t buffer_count) {
    this->buffer_count = buffer_count;
}

void DownloadUploadInfoPrinter::setCurrBufferIndex(uint64_t curr_buffer_idx) {
    this->curr_buffer_idx = curr_buffer_idx;
}

void DownloadUploadInfoPrinter::update() {
    updateCurrValues();
    updateLastValues();
    updateText();
}

void DownloadUploadInfoPrinter::updateCurrValues() {
    percentage = static_cast<float>(curr_buffer_idx) / buffer_count * 100.0f;
    bytes_per_sec = getBytesSentPerSec();
    remaining_time_sec = (bytes_per_sec == 0) ? 0 : getRemainingBytes() / bytes_per_sec;
    loading_bar.setValue(static_cast<uint16_t>(static_cast<float>(curr_buffer_idx) / buffer_count * LOADING_BAR_SIZE));
}

uint64_t DownloadUploadInfoPrinter::getBytesSentPerSec() const {
    return (curr_buffer_idx - last_buffer_idx) * buffer_size;
}

uint64_t DownloadUploadInfoPrinter::getRemainingBytes() const {
    return (buffer_count - curr_buffer_idx) * buffer_size;
}

void DownloadUploadInfoPrinter::updateLastValues() {
    last_buffer_idx = curr_buffer_idx;
}

void DownloadUploadInfoPrinter::updateText() {
    first_text =
        util::byte::toAutoUnit<float>(bytes_per_sec) + "/s, eta: " + util::string::secondsToDHMS(remaining_time_sec);
    second_text =
        loading_bar.getText() + ' ' + ((std::isnan(percentage)) ? "0%" : util::string::formatFloat(percentage) + '%');
}

void DownloadUploadInfoPrinter::print() {
    printFirstTextSpaceFill();
    if (first_print) {
        first_print = false;
        console::out::inf("\n");
    }
    shell::cursor::moveUp();
    shell::cursor::moveUp();
    console::out::inf(first_text);
    console::out::inf(second_text + '\r');
    last_first_text_size = first_text.size();
    last_second_text_size = second_text.size();
}

void DownloadUploadInfoPrinter::printFirstTextSpaceFill() {
    if (first_text.size() < last_first_text_size) {
        console::out::inf(util::string::makeFilledString(last_first_text_size) + '\r', false);
    }
}

void DownloadUploadInfoPrinter::clear() {
    shell::cursor::moveUp();
    console::out::inf(util::string::makeFilledString(last_second_text_size) + '\r', false);
    shell::cursor::moveUp();
    console::out::inf(util::string::makeFilledString(last_first_text_size) + '\r', false);
}
