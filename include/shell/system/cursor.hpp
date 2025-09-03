#pragma once

#include "system/beg.hpp"

#include <stdint.h>
#include "util/vector2.hpp"

namespace shell::cursor {

extern Vector2<int16_t> getPosition();
extern void move(const Vector2<int16_t>& pos);

extern void moveUp(int16_t count = 1);
extern void moveDown(int16_t count = 1);
extern void _return();

extern void setVisible(bool visible);

} // namespace shell::cursor
