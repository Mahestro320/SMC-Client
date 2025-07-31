#include "io/console.hpp"
#include "shell/commands/clear.hpp"
#include "shell/system/cursor.hpp"

exit_code_t ClearCommand::run() {
    return (getConsoleHandle() && getConsoleModes() && setNewConsoleMode() && clearScreen() &&
            restoreOriginalConsoleMode())
               ? Success
               : Error;
}

bool ClearCommand::getConsoleHandle() {
    h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == INVALID_HANDLE_VALUE) {
        printError("can't get the console handle");
        return false;
    }
    return true;
}

bool ClearCommand::getConsoleModes() {
    DWORD current_mode{};
    if (!GetConsoleMode(h_console, &current_mode)) {
        printError("can't get the new console mode");
        return false;
    }
    original_mode = current_mode;
    current_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    new_mode = current_mode;
    return true;
}

bool ClearCommand::setNewConsoleMode() {
    if (!SetConsoleMode(h_console, new_mode)) {
        printError("can't set the new console mode");
        return false;
    }
    return true;
}

bool ClearCommand::clearScreen() const {
    DWORD written{};
    if (!WriteConsoleA(h_console, SEQ_CLR_VISIBLE_SCRN, static_cast<DWORD>(strlen(SEQ_CLR_VISIBLE_SCRN)), &written,
                       nullptr) ||
        !WriteConsoleA(h_console, SEQ_CLR_SCROLLBACK, static_cast<DWORD>(strlen(SEQ_CLR_SCROLLBACK)), &written,
                       nullptr)) {
        printError("write error");
        return false;
    }
    shell::cursor::move(Vector2<int16_t>(0, 0));
    return true;
}

bool ClearCommand::restoreOriginalConsoleMode() const {
    if (!SetConsoleMode(h_console, original_mode)) {
        printError("can't restore original console mode");
        return false;
    }
    return true;
}

void ClearCommand::printError(const std::string& text) const {
    console::out::err("error while clearing screen: " + text);
}
