#include "shell/commands/quit.hpp"

#include "common.hpp"
#include "io/console.hpp"

exit_code_t QuitCommand::run() {
    console::out::verbose("exiting the shell");
    common::is_running = false;
    return Success;
}
