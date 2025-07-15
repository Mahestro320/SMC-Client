# Changelog

## Version 0.3.0

- adding `download` command
- adding env var resolver for commands and config file's value
- adding resolved view of config variables for command `conf get <key>`
- adding key in config file: downloads.output
- use boost::error_code instead of try-catch blocks in case of error in the connection with the server
- adding function console::out::err(const std::exception& e)
- adding request handler `io_get_file_size` and `io_get_file_content`
- inclusions were organized and "backward includes" were used
- adding `short_name` param in commands_infos
- adding `short_name` view in `help` command
- renaming command `cls` to `clear`
- bug fixed: when reconnecting, the client state is now reset
- replacing config file key `server.connect_at_launch` by `client.connect_at_launch`
- replacing config file key `stdout.verbose` by `shell.verbose`
- adding key in config file: `shell.print_addr_prefix`
- renaming function `void Client::closeSocket(void)` to `void Client::close(void)`
- renaming command `server` to `client`
- replacing config file key `downloads.output` by `commands.download.output`
- adding key in config file: `commands.download.buffer_size`

## Version 0.3.1

- When `download` command is interrupted by an error, the `info thread` is now correctely stopped
- The `info thread` of `download` command is now starting at the good time
