#include "client/UDPClient.h"
#include "core/constants.h"
#include "core/Message.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

using namespace jungle;

UDPClient::UDPClient(const std::string &t_addr, int t_serverPort)
    : m_addr{t_addr}, m_serverPort{t_serverPort}, m_socketfd{-1}, m_stop{false}, m_chat{}
{
}

UDPClient::~UDPClient()
{
    close(m_socketfd);
}

bool UDPClient::init()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(m_addr.c_str());
    memset(&(serverAddr.sin_zero), '\0', 8);

    m_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socketfd < 0)
    {
        std::cerr << "Failed to get socket file descriptor." << std::endl;
        close(m_socketfd);
        return false;
    }

    if (connect(m_socketfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Failed to connect to remote server." << std::endl;
        close(m_socketfd);
        return false;
    }

    return true;
}

void UDPClient::login(const std::string &t_username)
{
    m_username = t_username;
    std::string cmdLoginBuffer(char(MSG_LOGIN_TYPE) + m_username);
    send(m_socketfd, cmdLoginBuffer.c_str(), cmdLoginBuffer.size(), 0);
}

void UDPClient::start()
{
    m_chat.render();

    std::thread receiverThread(&UDPClient::receiver, this);
    std::thread senderThread(&UDPClient::sender, this);

    senderThread.join();
    receiverThread.join();

    close(m_socketfd);
}

void UDPClient::sender()
{
    char buffer[BUF_SIZE];
    MessageRequest message = {0};
    std::string sendBuffer;

    while (!m_stop)
    {
        bzero(buffer, BUF_SIZE);
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\r\n")] = 0; // Clear EOL. Works for LF, CR, CRLF, LFCR, ...

        m_chat.render();

        sendBuffer = buildMessage(m_username, buffer);
        send(m_socketfd, sendBuffer.c_str(), sendBuffer.size(), 0);

        if (strcmp(buffer, ":exit") == 0)
        {
            m_stop = true;
        }
    }
}

void UDPClient::receiver()
{
    char receiveBuffer[BUF_SIZE];
    int nbytes;
    Message message;

    while (!m_stop)
    {
        bzero(receiveBuffer, BUF_SIZE);
        nbytes = 0;
        message = {0};

        if ((nbytes = recv(m_socketfd, receiveBuffer, BUF_SIZE - 1, 0)) < 0)
        {
            m_chat.setServerIsOn(false);
            m_chat.render();
            continue;
        }
        m_chat.setServerIsOn(true);
        receiveBuffer[nbytes] = '\0';

        message.fromString(std::string(receiveBuffer));
        if (message.type == MSG_LOGOUT_TYPE && m_stop)
        {
            m_chat.bye();
            return;
        }
        else
        {
            m_chat.receive(message);
        }

        m_chat.render();
    }
}

std::string UDPClient::buildMessage(const std::string &t_username, std::string buffer) const
{
    MessageRequest message = {0};

    if (buffer == ":exit")
    {
        return char(MSG_LOGOUT_TYPE) + t_username;
    }
    else if (buffer[0] == ':')
    {
        message = {
            MSG_REMOVE_TEXT_TYPE,
            t_username,
            buffer.substr(1),
        };
    }
    else
    {
        message = {
            MSG_SEND_TEXT_TYPE,
            t_username,
            buffer,
        };
    }

    return message.toString();
}
