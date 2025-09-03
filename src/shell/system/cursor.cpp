#include "shell/system/cursor.hpp"

#include <windows.h>

Vector2<int16_t> shell::cursor::getPosition() {
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return Vector2<int16_t>{csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y};
}

void shell::cursor::move(const Vector2<int16_t>& pos) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{pos.x, pos.y});
}

void shell::cursor::moveUp(int16_t count) {
    Vector2<int16_t> pos{getPosition()};
    pos.y -= count;
    move(pos);
}

void shell::cursor::moveDown(int16_t count) {
    Vector2<int16_t> pos{getPosition()};
    pos.y += count;
    move(pos);
}

void shell::cursor::_return() {
    Vector2<int16_t> pos{getPosition()};
    pos.x = 0;
    move(pos);
}

void shell::cursor::setVisible(bool visible) {
    HANDLE h_console{GetStdHandle(STD_OUTPUT_HANDLE)};
    CONSOLE_CURSOR_INFO cursor_info{};
    GetConsoleCursorInfo(h_console, &cursor_info);
    cursor_info.bVisible = visible;
    SetConsoleCursorInfo(h_console, &cursor_info);
}
