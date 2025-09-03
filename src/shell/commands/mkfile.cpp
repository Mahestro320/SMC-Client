#include "shell/commands/mkfile.hpp"

#include "network/request/handlers/io_create_file.hpp"

exit_code_t MkFileCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    IOCreateFileRH handler{};
    handler.setClient(client);
    handler.setFilePath(args[0]);
    return (handler.run()) ? Success : Error;
}
