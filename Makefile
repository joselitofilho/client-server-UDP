CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address
LIBS=-pthread
BIN_DIR = ./bin
SRC_DIR = ./src
LIBS_DIR = ./libs
TEST_DIR = ./tests
PROGRAMS_DIR = ./programs
INCLUDE_PATHS = -I$(SRC_DIR)

GTEST_HOME = $(LIBS_DIR)/gtest
GTEST_LIBRARY_PATH = $(GTEST_HOME)/libs
GTESTFLAGS = -I$(GTEST_HOME)/include -L$(GTEST_LIBRARY_PATH) -lgtest -lgtest_main -pthread

CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server
TEST_EXEC = $(BIN_DIR)/tests

CONTAINER_NAME = client-server-udp

SERVER_PORT = 30000

.PHONY: all build client server test run-client run-server run-test clean

all: build

build: client server

client: $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(LIBS) $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp -o $(CLIENT_EXEC)

server: $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp -o $(SERVER_EXEC)

test: $(SRC_DIR)/server/*.cpp $(TEST_DIR)/main.cpp
	if [ ! -d "$(GTEST_HOME)" ]; then \
		docker exec -it $(CONTAINER_NAME) bash -c \
			"wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz; \
			tar xf release-1.8.0.tar.gz; \
			rm release-1.8.0.tar.gz; \
			cd googletest-release-1.8.0; \
			cmake -DBUILD_SHARED_LIBS=ON .; \
			make; \
			mkdir -p ../libs/gtest/include; \
			cp -a googletest/include/gtest/ ../libs/gtest/include; \
			mkdir -p ../libs/gtest/libs; \
			cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so ../libs/gtest/libs; \
			cd ..; \
			rm -r googletest-release-1.8.0/;"; \
    fi
	docker exec -it $(CONTAINER_NAME) bash -c \
		"$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(GTESTFLAGS) $(SRC_DIR)/server/*.cpp $(TEST_DIR)/*.cpp -o $(TEST_EXEC)"

run-client: $(CLIENT_EXEC)
	@$(CLIENT_EXEC) $(SERVER_PORT) $(CLIENT_NAME)

run-server: $(SERVER_EXEC)
	@$(SERVER_EXEC) $(SERVER_PORT)

run-test: $(TEST_EXEC)
	docker exec -it $(CONTAINER_NAME) bash -c "LD_LIBRARY_PATH=$(GTEST_LIBRARY_PATH) $(TEST_EXEC)"

clean:
	rm -rf $(CLIENT_EXEC) $(SERVER_EXEC) $(TEST_EXEC) *.o
