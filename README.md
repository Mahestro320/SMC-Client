# SMC (Super Mega Cool) Client by Mahestro_320

The **SMC client** (_SMCC_) is a **CLI (console)** application that uses commands to connect to an **SMC server** and request data from it by sending queries.

## How to compile

The client is programmed in **C++ (ISO C++20 Standard)**
To compile it, you can use **Visual Studio** or the **basic C++ compiler**

> [!WARNING]
> It needs **Boost 1.88.0+ to work**. To do this, **put the Boost include folder in `external` directory at root**

## How to connect

To connect to a server, you can modify the `server.address` and `server.port` keys of the configuration file **(more explanations below)**

## Configuration file

The configuration file is stored at `data\config.ini`

> [!NOTE]
> Booleans are represented with a **0** for **false** and **1** for **true**

Here are the file keys and their descriptions:

```ini
[server]
address=example.com    ; This is the address the client will try to connect to (ip or domain name)
port=500               ; This is the port on which the target server accepts connections
connect_at_launch=1    ; Indicates whether the client should connect when the application is launched

[stdout]
verbose=1              ; Indicates whether the output is verbose or not
```

> [!NOTE]
> Each values ​​in the ini file above are examples
