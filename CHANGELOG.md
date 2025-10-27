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

## Version 0.3.2

- moving `constexpr` variable `Shell::VERSION` to `common::VERSION`
- renaming `conf` command option `printvals` to `print`
- adding `infos` command

## Version 0.3.3

- removing incorrect usage text `<s=<buffer size> (optional)>` for command `download`
- refactoring `chdir` command
- renaming function `void IOGetRealPathRH::setInputPath(const std::filesystem::path& path)` to `void IOGetRealPathRH::setPath(const std::filesystem::path& path)`
- refactoring `clear` command
- refactoring `client` command
- refactoring `config` command
- renaming `conf` command option from `updatefile` to `save`
- the resolved view isn't printed if the resolved value have no changes from the value for `conf get <key>` command
- refactoring `download` command
- refactoring class `DownloadInfoPrinter`
- moving variable `common::VERSION` to `constants::VERSION` in `constants.hpp` new file
- refactoring class `LoginRH`
- making `Config` class constructor private
- refactoring code of file `directories.hpp`
- modifying start message
- adding loading bar for `download` command
- bug correction when an error occurs in the `Writer Thread` of command `download`
- replacing flag value `0xAB` to `0x00` for variable `IOGetFileContentRH::NEXT_BUFFER_FLAG`
- adding flag `IOGetFileContentRH::STOP_FLAG`
- modifying long description for command `client`: removing line return at end
- adding signal handles
- refactoring `io_get_complete_path` request handler
- refactoring `login` request handler
- refactoring `clear` command and removing clear bug 
- adding architecture check
- x86-64 compatibility

## Version 0.4.0

- removing bug of architecture check by always importing `system/beg.hpp`
- source files now have its header file include at first
- replacing text `unknown` by `undefined` in `config`
- removing text `<output path (optional)>` for command long description of `upload`
- separating declaration / implementation templates using `ipp` files
- refactoring `shell` code
- adding exit code `InvalidArgs` and `Silent`
- adding command `syscmd`
- adding command time displaying
- adding command `print`
- the `download` command downloads and writes on a single thread
- the `download` command can now download folders
- refactoring some request handlers and commands
- adding functions in `network`
- replacing ternary operator `/* cond */ ? /* if true */ : /* if false */` by `(/* cond */) ? /* if true */ : /* if false */`
- adding command `mkfile`
- adding request handler `io_create_file`
- adding command `mkdir`
- adding request handler `io_create_directory`
- can now use simple quotes and double quotes in shell command
- adding command `upload`
- adding request handler `io_delete_file`
- adding command `delete`
- adding request handler `get_version`
- adding command `server-ver`
