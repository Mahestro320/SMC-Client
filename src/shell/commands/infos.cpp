#include "common.hpp"
#include "io/console.hpp"
#include "shell/commands/infos.hpp"

exit_code_t InfosCommand::run() {
    console::out::inf("SMC (Super Mega Cool) Client Shell v" + common::VERSION.toString() + " by Mahestro_320");
    return Success;
}
