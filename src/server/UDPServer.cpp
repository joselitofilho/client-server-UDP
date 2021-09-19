#include "server/UDPServer.h"
#include "core/constants.h"
#include "core/errors.h"
#include <arpa/inet.h>
#include <map>
#include <string.h>
#include <unistd.h>

UDPServer::UDPServer(const std::string &addr_, int port_, Controller &controller_)
    : addr(addr_), port(port_), socketfd(-1), controller(controller_)
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

void UDPServer::start()
{
    int nbytes;
    char requestBuffer[BUF_SIZE];
    int addressSize = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddr;
    Message responseMessage = {0};

    while (1)
    {
        bzero(requestBuffer, BUF_SIZE);

        if ((nbytes =
                 recvfrom(socketfd, requestBuffer, BUF_SIZE - 1, 0, (struct sockaddr *)&clientAddr,
                          (unsigned int *)&addressSize)) < 0)
        {
            close(socketfd);
            error("Reading data from socket.");
        }

        requestBuffer[nbytes] = '\0';

        responseMessage = controller.onRequestHandle(requestBuffer, clientAddr);
        if (responseMessage.type != MSG_INVALID_TYPE)
        {
            broadcast(controller.getLoggedUsers(), responseMessage.toString().c_str());
        }
    }
}

void UDPServer::broadcast(const SocketUsers &loggedUsers, const char *buffer) const
{
    std::map<std::string, struct sockaddr_in>::const_iterator it = loggedUsers.begin();
    while (it != loggedUsers.end())
    {
        if (sendto(socketfd, buffer, strlen(buffer), 0, (struct sockaddr *)&it->second, sizeof(struct sockaddr)) < 0)
        {
            close(socketfd);
            error("Writing to socket.");
        }
        ++it;
    }
}
