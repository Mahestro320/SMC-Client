#pragma once

#include <string>
#include <vector>

class CommandLine {
  private:
    std::string raw_line{};
    std::vector<std::string> tokenized_line{};

    void flushToken(std::string& item);

  public:
    CommandLine() = default;

    bool getLine();
    void tokenize();
    bool empty() const;
    const std::vector<std::string>& getTokens() const;
};
