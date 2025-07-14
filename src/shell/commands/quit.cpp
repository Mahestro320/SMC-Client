#include "common.hpp"
#include "io/console.hpp"
#include "shell/commands/quit.hpp"

exit_code_t QuitCommand::run() {
    console::out::verbose("exiting the shell");
    common::is_running = false;
    return Success;
}
