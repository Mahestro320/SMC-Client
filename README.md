# SMC (Super Mega Cool) Client v0.5.0 by Mahestro_320

The **SMC client** (_SMCC_) is a **CLI (console)** application which uses commands to connect to an
**SMC server** and request data from it by sending requests.

## How to compile

The client is programmed in **C++ (ISO C++20 Standard)**. To compile it, you can use
**Visual Studio** or the **basic C++ compiler**

> [!WARNING]
> It needs **Boost 1.88.0+ to work**. To do this, **put the Boost include folder in `external`
directory at root**

## Environnement variables

To insert an environnement variable in a string, type <[VARNAME]>

## How to connect

To connect to a server, you can modify the `server.address` and `server.port` keys of the
configuration file **(more explanations below)**

## Configuration file

The configuration file is stored at `data\config.ini`. To modify values ​​in the file, you can either
do it through the shell (with the `conf` command) or with a standard text editor

> [!NOTE]
> Booleans are represented with a **0** for **false** and **1** for **true**

Here are the file keys and their descriptions:

```ini
[server]

; This is the address the client will try to connect to (ip or domain name)
address=example.com

; This is the port on which the target server accepts connections
port=500

[client]

; Indicates whether the client should connect when the application is launched
connect_at_launch=1

[shell]

; Indicates whether the output is verbose or not
verbose=1

; Indicates whether the text `<address>:<ip>` before the command line should be displayed
print_addr_prefix=1

[commands]

; The default output for downloaded files
download_output=C:\Users\<USERNAME>\Downloads

; The size of each buffer that is received / sent (maximum is 65535)
download_buffer_size=4096
```

> [!NOTE]
> Each values ​​in the ini file above are examples
