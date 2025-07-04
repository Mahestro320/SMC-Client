#pragma once

#include "../command.hpp"

class LoginCommand final : public Command {
  private:
    std::string username{}, password{};

    bool getArgumentsValues();
    bool checkUsername() const;
    bool checkPassword() const;

  public:
    LoginCommand() = default;

    exit_code_t run() override;
};
