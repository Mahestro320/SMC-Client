#include "shell/commands/syscmd.hpp"

#include "io/console.hpp"
#include "network/request/handlers/run_syscmd.hpp"

exit_code_t SyscmdCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    RunSyscmdRH handler{};
    handler.setClient(client);
    handler.setCommand(args[0]);
    if (!handler.run()) {
        return Error;
    }
    console::out::inf(handler.getOutput(), false);
    return Success;
}
