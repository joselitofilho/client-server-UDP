#include "server/UDPServer.h"
#include "core/constants.h"
#include "core/errors.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UDPServer::UDPServer(const std::string &addr_, int port_)
    : addr(addr_), port(port_), socketfd(-1)
{
}

UDPServer::~UDPServer()
{
    close(socketfd);
}

bool UDPServer::init()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    memset(&(serverAddr.sin_zero), '\0', 8);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd < 0)
    {
        std::cerr << "Failed to get socket file descriptor." << std::endl;
        close(socketfd);
        return false;
    }

    if (bind(socketfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Failed to bind on socket." << std::endl;
        close(socketfd);
        return false;
    }

    std::cout << "Server running on port " << port << std::endl;

    return true;
}

void UDPServer::start() const
{
    int nbytes;
    char requestBuffer[BUF_SIZE];
    char responseBuffer[BUF_SIZE + USERNAME_LEN];
    int addressSize = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddr;

    bzero(requestBuffer, BUF_SIZE);
    bzero(responseBuffer, BUF_SIZE + USERNAME_LEN);

    while (1)
    {
        if ((nbytes =
                 recvfrom(socketfd, requestBuffer, BUF_SIZE - 1, 0, (struct sockaddr *)&clientAddr,
                          (unsigned int *)&addressSize)) < 0)
        {
            close(socketfd);
            error("Reading data from socket.");
        }

        requestBuffer[nbytes] = '\0';
        std::cout << "Received message: " << requestBuffer;
    }
}
