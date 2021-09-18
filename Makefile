CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

BIN = ./bin
SRC = ./src
CLIENT_OUT = $(BIN)/client
SERVER_OUT = $(BIN)/server

all: client server

client: $(SRC)/client/main.cpp
	$(CXX) $(LDFLAGS) $(SRC)/client/main.cpp -o $(CLIENT_OUT)

server: $(SRC)/server/main.cpp
	$(CXX) $(LDFLAGS) $(SRC)/server/main.cpp -o $(SERVER_OUT)

clean:
	rm -rf $(CLIENT_OUT) $(SERVER_OUT)
