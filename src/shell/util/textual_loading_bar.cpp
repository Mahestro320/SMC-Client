#include "shell/util/textual_loading_bar.hpp"

TextualLoadingBar::TextualLoadingBar(uint16_t size, char fill_char, char empty_char)
    : size{size}, fill_char{fill_char}, empty_char{empty_char} {
}

void TextualLoadingBar::setValue(uint16_t value) {
    this->value = std::min(value, size);
}

void TextualLoadingBar::increment() {
    value = std::min(static_cast<uint16_t>(value + 1), size);
}

void TextualLoadingBar::decrement() {
    value = (value == 0) ? 0 : value - 1;
}

std::string TextualLoadingBar::getText() const {
    std::string bar_content{};
    for (uint16_t i{}; i < value; ++i) {
        bar_content += fill_char;
    }
    const uint16_t space_left{static_cast<uint16_t>(size - value)};
    for (uint16_t i{}; i < space_left; ++i) {
        bar_content += empty_char;
    }
    return '[' + bar_content + ']';
}
