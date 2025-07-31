#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "../command.hpp"

class ClearCommand final : public Command {
  private:
    static inline constexpr char SEQ_CLR_VISIBLE_SCRN[]{"\x1b[2J"};
    static inline constexpr char SEQ_CLR_SCROLLBACK[]{"\x1b[3J"};

    HANDLE h_console{};
    DWORD new_mode{};
    DWORD original_mode{};

    bool getConsoleHandle();
    bool getConsoleModes();
    bool setNewConsoleMode();
    bool clearScreen() const;
    bool restoreOriginalConsoleMode() const;

    void printError(const std::string& text) const;

  public:
    ClearCommand() = default;

    exit_code_t run() override;
};
