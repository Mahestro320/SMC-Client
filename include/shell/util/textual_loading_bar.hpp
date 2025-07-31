#pragma once

#include <string>

class TextualLoadingBar final {
  private:
    const char fill_char{}, empty_char{};
    const uint16_t size{};
    uint16_t value{};

  public:
    TextualLoadingBar(uint16_t size, char fill_char = '#', char empty_char = '-');

    void setValue(uint16_t value);
    void increment();
    void decrement();

    std::string getText() const;
};
