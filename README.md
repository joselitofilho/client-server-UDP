# client-server-UDP

This project implements the communication between client and server via User Datagram Protocol (UDP).

**Table of Contents**

- [Prerequisites](#prerequisites)
    - [Versions and Supportability](#versions-and-supportability)
- [Project structure](#project-structure)
- [Build](#build)
- [Run](#run)
- [Tests](#tests)

## Prerequisites

- [Make](https://www.gnu.org/software/make/) version 4.2+
- [Docker](https://docs.docker.com/install/) version 20.10+

_NOTE_: To facilitate the execution of commands configure the docker to run without sudo.
For more informations, visit the docker oficial documentation [Manage Docker as a non-root user](https://docs.docker.com/engine/install/linux-postinstall/).

### _Versions and Supportability_

Projects contain a [`Makefile`](Makefile) that will install tools at versions defined at creation time. Those tools are:

- [Redis](https://redis.io/)
- [googletest and googlemock](https://github.com/google/googletest)
- [hiredis](https://github.com/redis/hiredis)

## Project structure

| File or Folder | Description                                                              |
| -------------- | ------------------------------------------------------------------------ |
| bin/           | Where the application and test binaries will be generated.               |
| libs/          | Contains statically linkable, lib files and is part of a library.        |
| programs/      | Source code of the main application (client and server).                 |
| src/           | Where the source codes is.                                               |
| tests/         | All application tests.                                                   |
| Dockerfile     | Used to create a docker image to help us build and run the applications. |
| Makefile       | Contains a set of directives to build, test, and run the applications.   |

## Build

```bash
$ make
```

## Run

Run the server on default port:
```bash
$ make run-server
```
or run on a specific port:
```bash
$ make run-server SERVER_PORT=40000
```

Run the client on the server's default port and enter the username `USER_NAME`:
```bash
$ make run-client USER_NAME=Joselito
```
or run the client specifying the server port:
```bash
$ make run-client SERVER_PORT=40000 USER_NAME=Joselito
```

## Tests

```bash
$ make run-test
```
