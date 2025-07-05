# SMC (Super Mega Cool) Client by Mahestro_320

The SMC client (SMCC) is a CLI (console) application that uses commands to connect to an SMC server and request data from it by sending queries.

## How to compile

The client is programmed in C++ (ISO C++20 Standard)
To compile it, you can use Visual Studio or the basic C++ compiler

> [!WARNING]
> It needs Boost 1.88.0+ to work. To do this, put the Boost include folder in "external" directory at root

## How to connect

To connect to a server, you can modify the "server.address" and "server.port" keys of the "data/config.ini" configuration file using the shell or a standard text editor

## Configuration file

The configuration file is stored at `data\config.ini`

> [!NOTE]
> Booleans are represented with a 0 for false and 1 for true

Here are the file keys and their descriptions:

```ini
[server]
address:             This is the address the client will try to connect to (ip or domain name)
port:                This is the port on which the target server accepts connections
connect_at_launch:   Indicates whether the client should connect when the application is launched

[stdout]
verbose:             Indicates whether the output is verbose or not
```
