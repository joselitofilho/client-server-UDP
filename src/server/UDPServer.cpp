#include "server/UDPServer.h"
#include "core/constants.h"
#include <arpa/inet.h>
#include <map>
#include <string.h>
#include <unistd.h>

using namespace jungle;

UDPServer::UDPServer(const std::string &t_addr, int t_port, Controller &t_controller)
    : m_addr{t_addr}, m_port{t_port}, m_socketfd{-1}, m_controller{t_controller}
{
}

UDPServer::~UDPServer()
{
    close(m_socketfd);
}

bool UDPServer::init()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = inet_addr(m_addr.c_str());
    memset(&(serverAddr.sin_zero), '\0', 8);

    m_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socketfd < 0)
    {
        std::cerr << "Failed to get socket file descriptor." << std::endl;
        close(m_socketfd);
        return false;
    }

    if (bind(m_socketfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Failed to bind on socket." << std::endl;
        close(m_socketfd);
        return false;
    }

    std::cout << "Server running on port " << m_port << std::endl;

    return true;
}

void UDPServer::start() const
{
    int nbytes;
    char requestBuffer[BUF_SIZE];
    int addressSize = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddr;
    Message responseMessage;

    while (1)
    {
        bzero(requestBuffer, BUF_SIZE);
        responseMessage = {0};

        if ((nbytes =
                 recvfrom(m_socketfd, requestBuffer, BUF_SIZE - 1, 0, (struct sockaddr *)&clientAddr,
                          (unsigned int *)&addressSize)) < 0)
        {
            std::cerr << "Reading data from socket." << std::endl;
            close(m_socketfd);
            return;
        }
        requestBuffer[nbytes] = '\0';

        responseMessage = m_controller.onRequestHandle(requestBuffer, clientAddr);
        if (responseMessage.type != MSG_INVALID_TYPE)
        {
            broadcast(m_controller.getLoggedUsers(), responseMessage.toString().c_str());

            switch (responseMessage.type)
            {
            case MSG_LOGIN_TYPE:
            {
                sendMessages(m_controller.allMessages(), clientAddr);
            }
            break;
            case MSG_LOGOUT_TYPE:
            {
                sendMessage(responseMessage.toString().c_str(), clientAddr);
            }
            break;
            }
        }
    }
}

void UDPServer::broadcast(const SocketUsers &t_users, const char *t_buffer) const
{
    auto it = t_users.begin();
    while (it != t_users.end())
    {
        sendMessage(t_buffer, it->second);
        ++it;
    }
}

void UDPServer::sendMessage(const char *t_buffer, struct sockaddr_in t_clientAddr) const
{
    if (sendto(m_socketfd, t_buffer, strlen(t_buffer), 0, (struct sockaddr *)&t_clientAddr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Writing to socket." << std::endl;
    }
}

void UDPServer::sendMessages(const Messages &t_messages, struct sockaddr_in t_clientAddr) const
{
    auto it = t_messages.begin();
    while (it != t_messages.end())
    {
        sendMessage(it->second.toString().c_str(), t_clientAddr);
        ++it;
    }
}
