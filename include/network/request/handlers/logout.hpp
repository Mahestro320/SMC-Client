#pragma once

#include "system/beg.hpp"

#include "../handler.hpp"

class LogoutRH final : public RH {
  public:
    LogoutRH() = default;

    bool run() override;
};
