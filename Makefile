CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -g -fsanitize=address #-pedantic hiredis
LDFLAGS =  -fsanitize=address
LIBS=-pthread
BIN_DIR = ./bin
SRC_DIR = ./src
LIBS_DIR = ./libs
TEST_DIR = ./tests
PROGRAMS_DIR = ./programs
INCLUDE_PATHS = -I$(SRC_DIR)

HIREDIS_HOME = $(LIBS_DIR)/hiredis
HIREDIS_LIBRARY_PATH = $(HIREDIS_HOME)/lib
HIREDISFLAGS = -I$(HIREDIS_HOME)/include -L$(HIREDIS_LIBRARY_PATH) -lhiredis

GTEST_HOME = $(LIBS_DIR)/gtest
GTEST_LIBRARY_PATH = $(GTEST_HOME)/lib
GTESTFLAGS = -I$(GTEST_HOME)/include -L$(GTEST_LIBRARY_PATH) -lgtest -lgtest_main -pthread

CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server
TEST_EXEC = $(BIN_DIR)/tests

IMAGE_NAME = cpp-dev
CONTAINER_NAME = client-server-udp

DOCKER_EXEC = docker exec -it $(CONTAINER_NAME) bash -c

SERVER_PORT = 30000

.PHONY: all build containers cpp-dev client server test run-client run-server run-test clean gtest hiredis redis

all: build

build: client server containers

CPP_DEV_RUNNING = $(shell docker ps -a | grep $(CONTAINER_NAME) | wc -l)
containers:
	if [ $(CPP_DEV_RUNNING) -eq 0 ]; then \
		docker build -t $(IMAGE_NAME) . ; \
	fi

cpp-dev: containers
	if [ $(CPP_DEV_RUNNING) -eq 0 ]; then \
		docker run -v $(pwd):/src -w /src --label com.docker.compose.project=development --network=host --rm -itd --name $(CONTAINER_NAME) $(IMAGE_NAME); \
	fi

client: $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(LIBS) $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp -o $(CLIENT_EXEC)"

server: $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp redis
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(HIREDISFLAGS) $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp -o $(SERVER_EXEC)"

test: $(SRC_DIR)/server/*.cpp $(TEST_DIR)/main.cpp gtest
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(HIREDISFLAGS) $(GTESTFLAGS) $(SRC_DIR)/server/*.cpp $(TEST_DIR)/*.cpp -o $(TEST_EXEC)"

run-client: $(CLIENT_EXEC)
	$(DOCKER_EXEC) "$(CLIENT_EXEC) $(SERVER_PORT) $(CLIENT_NAME)"

run-server: $(SERVER_EXEC)
	$(DOCKER_EXEC) "LD_LIBRARY_PATH=$(HIREDIS_LIBRARY_PATH) $(SERVER_EXEC) $(SERVER_PORT)"

run-test: $(TEST_EXEC)
	$(DOCKER_EXEC) "LD_LIBRARY_PATH=$(HIREDIS_LIBRARY_PATH):$(GTEST_LIBRARY_PATH) $(TEST_EXEC)"

clean:
	rm -rf $(CLIENT_EXEC) $(SERVER_EXEC) $(TEST_EXEC) *.o

gtest: cpp-dev
	if [ ! -d "$(GTEST_HOME)" ]; then \
		$(DOCKER_EXEC) \
			"wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz; \
			tar xf release-1.8.0.tar.gz; \
			rm release-1.8.0.tar.gz; \
			cd googletest-release-1.8.0; \
			cmake -DBUILD_SHARED_LIBS=ON .; \
			make; \
			mkdir -p ../libs/gtest/include; \
			cp -a googletest/include/gtest/ ../libs/gtest/include; \
			mkdir -p ../libs/gtest/lib; \
			cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so ../libs/gtest/lib; \
			cd ..; \
			rm -r googletest-release-1.8.0/;"; \
    fi

hiredis: cpp-dev
	if [ ! -d "$(HIREDIS_HOME)" ]; then \
		$(DOCKER_EXEC) \
			"git clone https://github.com/redis/hiredis.git; \
			mkdir -p ./libs/hiredis; \
			cd hiredis; \
			make PREFIX=../libs/hiredis; \
			make PREFIX=../libs/hiredis install; \
			cd ..; \
			rm -r hiredis/;"; \
    fi

REDIS_RUNNING = $(shell docker ps -a | grep redis | wc -l)
redis:
	if [ $(REDIS_RUNNING) -eq 0 ]; then \
		docker run --label com.docker.compose.project=development -d -p 6379:6379 -e ALLOW_EMPTY_PASSWORD=yes --name redis bitnami/redis:latest; \
	fi