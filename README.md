# mchat-server
A simple terminal chat application written in C, including a TCP server and an ncurses-based client.

## Build from sources
```bash
git clone https://github.com/Yuyu-1115/mchat-server.git
cd mchat-server
make
```

The binaries are generated at:
- `build/mchat_server`
- `build/mchat_client`

## Run
Start the server:
```bash
make run
```

Then start a client in another terminal:
```bash
make client
```

The server listens on port `8080`.

## Generate compile_commands.json
```bash
bear -- make
```
