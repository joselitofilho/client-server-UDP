CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address
LIBS=-pthread
BIN_DIR = ./bin
SRC_DIR = ./src
OBJ_DIR = ./obj
INCLUDE_PATHS = -I$(SRC_DIR)

CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server

SERVER_PORT = 30000

.PHONY: all
all: client server

.PHONY: client
client: $(SRC_DIR)/client/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(LIBS) $(SRC_DIR)/client/*.cpp -o $(CLIENT_EXEC)

.PHONY: server
server: $(SRC_DIR)/server/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/server/*.cpp -o $(SERVER_EXEC)

.PHONY: run-server
run-server: $(SERVER_EXEC)
	@$(SERVER_EXEC) $(SERVER_PORT)

.PHONY: run-client
run-client: $(CLIENT_EXEC)
	@$(CLIENT_EXEC) $(SERVER_PORT) $(CLIENT_NAME)

.PHONY: clean
clean:
	rm -rf $(CLIENT_EXEC) $(SERVER_EXEC)
