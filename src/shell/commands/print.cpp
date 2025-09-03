#include "shell/commands/print.hpp"

#include "io/console.hpp"

exit_code_t PrintCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    console::out::inf(args[0]);
    return Silent;
}
