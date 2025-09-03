#include "shell/commands/mkdir.hpp"

#include "network/request/handlers/io_create_directory.hpp"

exit_code_t MkDirCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    IOCreateDirectoryRH handler{};
    handler.setClient(client);
    handler.setDirectoryPath(args[0]);
    return (handler.run()) ? Success : Error;
}
