CXX = g++
CXXFLAGS =      -Wall -Werror -Wextra -std=c++17 -g -fsanitize=address
LDFLAGS =       -fsanitize=address
CXXFLAGS_TEST = -Wall -Werror -Wextra -std=c++17 -g -w -Wno-error=deprecated-copy

BIN_DIR  =     ./bin
SRC_DIR  =     ./src
LIBS_DIR =     ./libs
TEST_DIR = 	   ./tests
PROGRAMS_DIR = ./programs

INCLUDE_PATHS = -I$(SRC_DIR)

HIREDIS_HOME = $(LIBS_DIR)/hiredis
HIREDIS_LIBRARY_PATH = $(HIREDIS_HOME)/lib
HIREDISFLAGS = -I$(HIREDIS_HOME)/include -L$(HIREDIS_LIBRARY_PATH) -lhiredis

GTEST_HOME = $(LIBS_DIR)/gtest
GMOCK_HOME = $(LIBS_DIR)/gmock
GTEST_LIBRARY_PATH = $(GTEST_HOME)/lib
GMOCK_LIBRARY_PATH = $(GMOCK_HOME)/lib
GTESTFLAGS = -I$(GTEST_HOME)/include -I$(GMOCK_HOME)/include -L$(GTEST_LIBRARY_PATH) -L$(GMOCK_LIBRARY_PATH) -lgtest -lgtest_main -lgmock -pthread

CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server
TEST_EXEC =   $(BIN_DIR)/tests

IMAGE_NAME =     builder
CONTAINER_NAME = client-server-udp

DOCKER_EXEC = docker exec -it $(CONTAINER_NAME) bash -c

SERVER_PORT = 30000
PWD = $(shell pwd)

.PHONY: all build builder builder-container client server test run-client run-server run-test gtest hiredis redis clean clean-all

all: build

build: builder-container client server test redis

BUILDER_IMAGE_CREATED = $(shell docker images -q $(IMAGE_NAME) | wc -l)
builder:
	if [ $(BUILDER_IMAGE_CREATED) -eq 0 ]; then \
		docker build -t $(IMAGE_NAME) . ; \
	fi

builder-container: builder
	-docker run -v $(PWD):/src -w /src --label com.docker.compose.project=development --network=host --rm -itd --name $(CONTAINER_NAME) $(IMAGE_NAME)

client: builder-container $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) -pthread $(SRC_DIR)/client/*.cpp $(PROGRAMS_DIR)/client/main.cpp -o $(CLIENT_EXEC)"

server: hiredis $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp redis
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(HIREDISFLAGS) $(SRC_DIR)/server/*.cpp $(PROGRAMS_DIR)/server/main.cpp -o $(SERVER_EXEC)"

test: gtest $(SRC_DIR)/server/*.cpp $(TEST_DIR)/main.cpp gtest
	$(DOCKER_EXEC) "$(CXX) $(CXXFLAGS_TEST) $(INCLUDE_PATHS) $(HIREDISFLAGS) $(GTESTFLAGS) $(SRC_DIR)/server/*.cpp $(TEST_DIR)/*.cpp -o $(TEST_EXEC)"

$(CLIENT_EXEC): client
run-client: $(CLIENT_EXEC)
	$(DOCKER_EXEC) "$(CLIENT_EXEC) $(SERVER_PORT) $(USER_NAME)"

$(SERVER_EXEC): server redis
run-server: $(SERVER_EXEC)
	$(DOCKER_EXEC) "LD_LIBRARY_PATH=$(HIREDIS_LIBRARY_PATH) $(SERVER_EXEC) $(SERVER_PORT)"

$(TEST_EXEC): test
run-test: $(TEST_EXEC)
	$(DOCKER_EXEC) "LD_LIBRARY_PATH=$(HIREDIS_LIBRARY_PATH):$(GTEST_LIBRARY_PATH):$(GMOCK_LIBRARY_PATH) $(TEST_EXEC)"

gtest: builder-container
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
			mkdir -p ../libs/gmock/include; \
			cp -a googlemock/include/gmock/ ../libs/gmock/include; \
			mkdir -p ../libs/gtest/lib; \
			cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so ../libs/gtest/lib; \
			mkdir -p ../libs/gmock/lib; \
			cp -a googlemock/libgmock_main.so googlemock/libgmock.so ../libs/gmock/lib; \
			cd ..; \
			rm -r googletest-release-1.8.0/;"; \
    fi

hiredis: builder-container
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

redis:
	-docker run --rm --label com.docker.compose.project=development -d -p 6379:6379 -e ALLOW_EMPTY_PASSWORD=yes --name redis bitnami/redis:latest

clean:
	rm -rf $(CLIENT_EXEC) $(SERVER_EXEC) $(TEST_EXEC)

clean-all: clean
	rm -rf release-1.8.0.tar.gz googletest-release-1.8.0/ hiredis/ \
		$(GTEST_HOME) $(GMOCK_HOME) $(HIREDIS_HOME)
	-docker stop $(CONTAINER_NAME)
	-docker stop redis
