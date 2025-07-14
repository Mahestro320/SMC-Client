#pragma once

#include "../handler.hpp"

class LogoutRH final : public RH {
  public:
    LogoutRH() = default;

    bool run() override;
};
