#include <WinSock2.h>
#include <Windows.h>
#include "io/console.hpp"
#include "shell/commands/clear.hpp"

exit_code_t ClearCommand::run() {
    HANDLE console_handle{GetStdHandle(STD_OUTPUT_HANDLE)};
    if (console_handle == INVALID_HANDLE_VALUE) {
        console::out::err("error: can't clear the screen: failed to get console handle");
        return Error;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (!GetConsoleScreenBufferInfo(console_handle, &csbi)) {
        console::out::err("error: can't clear the screen: failed to get console buffer info");
        return Error;
    }

    DWORD console_size{static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y)};
    DWORD chars_written{};
    constexpr COORD home{0, 0};

    FillConsoleOutputCharacter(console_handle, ' ', console_size, home, &chars_written);
    FillConsoleOutputAttribute(console_handle, csbi.wAttributes, console_size, home, &chars_written);
    SetConsoleCursorPosition(console_handle, home);

    return Success;
}
