#include "server/UDPServer.h"
#include "core/constants.h"
#include "core/errors.h"
#include "MessageHandler.h"
#include <arpa/inet.h>
#include <map>
#include <string.h>
#include <unistd.h>

UDPServer::UDPServer(const std::string &addr_, int port_)
    : addr(addr_), port(port_), socketfd(-1)
{
}

UDPServer::~UDPServer()
{
    close(socketfd);
}

void UDPServer::broadcast(std::map<std::string, struct sockaddr_in> &loggedUsers, const char *message) const
{
    std::map<std::string, struct sockaddr_in>::iterator it = loggedUsers.begin();
    while (it != loggedUsers.end())
    {
        if (sendto(socketfd, message, strlen(message), 0, (struct sockaddr *)&it->second, sizeof(struct sockaddr)) < 0)
        {
            close(socketfd);
            error("Writing to socket.");
        }
        ++it;
    }
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
    MessageHandler messageHandler;
    Message requestMessage = {0};
    Message responseMessage = {0};
    std::map<std::string, struct sockaddr_in> loggedUsers;

    bzero(requestBuffer, BUF_SIZE);

    while (1)
    {
        responseMessage = {0};

        if ((nbytes =
                 recvfrom(socketfd, requestBuffer, BUF_SIZE - 1, 0, (struct sockaddr *)&clientAddr,
                          (unsigned int *)&addressSize)) < 0)
        {
            close(socketfd);
            error("Reading data from socket.");
        }

        requestBuffer[nbytes] = '\0';
        requestMessage = messageHandler.parseMessage(requestBuffer);
        std::cout << "From: " << requestMessage.username << " - Message: " << requestMessage.text << std::endl;

        // TODO: repository

        switch (requestMessage.type)
        {
        case MSG_LOGIN_TYPE:
        {
            loggedUsers.insert_or_assign(requestMessage.username, clientAddr);
            responseMessage = {
                MSG_SEND_TEXT_TYPE,
                "server",
                requestMessage.username + " is logged in.",
            };
            broadcast(loggedUsers, responseMessage.toString().c_str());
        }
        break;
        }
    }
}
