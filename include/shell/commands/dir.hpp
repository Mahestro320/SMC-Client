#pragma once

#include "system/beg.hpp"

#include "../command.hpp"
#include "io/file_info.hpp"

class DirCommand final : public Command {
  private:
    static inline constexpr uint64_t TYPE_SPACES_COUNT{13};
    static inline constexpr uint64_t SIZE_SPACES_COUNT{10};
    std::vector<FileInfo> data{};

    void printData() const;

  public:
    DirCommand() = default;

    exit_code_t run() override;
};
