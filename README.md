# client-server-UDP

This project implements the communication between client and server via User Datagram Protocol (UDP).

**Table of Contents**

- [Build](#build)
- [Run](#run)

## Build

```bash
$ make
```

Server:
```bash
$ make server
```

Client:
```bash
$ make client
```

## Run

Server:
```bash
$ make run-server SERVER_PORT=30000
```
or
```bash
$ ./bin/server 30000
```

Client:
```bash
$ make run-client SERVER_PORT=30000 CLIENT_NAME=Joselito
```
or
```bash
$ ./bin/client 30000 Joselito
```

## Test
```bash
$ make test
$ make run-test
```
