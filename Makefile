CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = ./src
INCLUDE_PATHS = -I$(SRC)

BIN = ./bin
CLIENT_OUT = $(BIN)/client
SERVER_OUT = $(BIN)/server

SERVER_PORT = 30000

.PHONY: all
all: client server

.PHONY: client
client: $(SRC)/client/main.cpp
	$(CXX) $(LDFLAGS) $(INCLUDE_PATHS) $(SRC)/client/main.cpp -o $(CLIENT_OUT)

.PHONY: server
server: $(SRC)/server/main.cpp
	$(CXX) $(LDFLAGS) $(INCLUDE_PATHS) $(SRC)/server/main.cpp -o $(SERVER_OUT)

.PHONY: run-server
run-server: $(SERVER_OUT)
	@$(SERVER_OUT) $(SERVER_PORT)

.PHONY: run-client
run-client: $(CLIENT_OUT)
	@$(CLIENT_OUT) $(SERVER_PORT) $(CLIENT_NAME)

.PHONY: clean
clean:
	rm -rf $(CLIENT_OUT) $(SERVER_OUT)
