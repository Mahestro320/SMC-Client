#pragma once

#include "system/beg.hpp"

#include <string>

#include "../handler.hpp"

class GetVersionRH final : public RH {
  private:
    std::string version{};

  public:
    GetVersionRH() = default;

    bool run() override;

    const std::string& getValue() const;
};
