#include "shell/commands/infos.hpp"

#include "constants.hpp"
#include "io/console.hpp"

exit_code_t InfosCommand::run() {
    console::out::inf(constants::INFO_MESSAGE);
    return Success;
}
