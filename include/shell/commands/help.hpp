#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

struct CommandInfos;

class HelpCommand final : public Command {
  private:
    static inline constexpr uint64_t NAME_SPACES_COUNT{15};
    static inline constexpr uint64_t SHORT_NAME_SPACES_COUNT{15};
    static inline constexpr uint64_t SHORT_DESCRIPTION_SPACES_COUNT{50};
    static inline constexpr uint64_t CAN_USE_OFFLINE_SPACES_COUNT{20};

    bool canSee(const CommandInfos& cmd) const;
    void printAllCommandsInfos() const;
    void printBeginMessage() const;
    void printCommandInfos(const CommandInfos& cmd) const;
    exit_code_t printLongDescription() const;
    void printLongDescriptionOf(const CommandInfos& cmd) const;

  public:
    HelpCommand() = default;

    exit_code_t run() override;
};
