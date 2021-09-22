#include "client/UDPClient.h"
#include "core/constants.h"
#include "core/Message.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

UDPClient::UDPClient(const std::string &addr_, int serverPort_)
    : addr(addr_), serverPort(serverPort_), socketfd(-1), stop(false), chat()
{
}

UDPClient::~UDPClient()
{
    close(socketfd);
}

bool UDPClient::init()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    memset(&(serverAddr.sin_zero), '\0', 8);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd < 0)
    {
        std::cerr << "Failed to get socket file descriptor." << std::endl;
        close(socketfd);
        return false;
    }

    if (connect(socketfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        std::cerr << "Failed to connect to remote server." << std::endl;
        close(socketfd);
        return false;
    }

    return true;
}

void UDPClient::login(const std::string &username_)
{
    username = username_;
    std::string cmdLoginBuffer(char(MSG_LOGIN_TYPE) + username);
    if (send(socketfd, cmdLoginBuffer.c_str(), cmdLoginBuffer.size(), 0) < 0)
    {
        std::cerr << "Failed to send message." << std::endl;
        close(socketfd);
        stop = true;
    }
}

void UDPClient::start()
{
    std::thread senderThread(&UDPClient::sender, this);
    std::thread receiverThread(&UDPClient::receiver, this);

    senderThread.join();
    receiverThread.join();

    close(socketfd);
}

void UDPClient::sender()
{
    char buffer[BUF_SIZE];
    MessageRequest message = {0};
    std::string sendBuffer;

    std::map<long long, Message>::const_iterator it;

    while (!stop)
    {
        bzero(buffer, BUF_SIZE);
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\r\n")] = 0; // Clear EOL. Works for LF, CR, CRLF, LFCR, ...

        chat.render();

        sendBuffer = buildMessage(username, buffer);
        if (send(socketfd, sendBuffer.c_str(), sendBuffer.size(), 0) < 0)
            std::cerr << "Failed to send message." << std::endl;

        if (strcmp(buffer, ":exit") == 0)
            stop = true;
    }
}

void UDPClient::receiver()
{
    char receiveBuffer[BUF_SIZE];
    int nbytes;
    Message message;

    while (!stop)
    {
        bzero(receiveBuffer, BUF_SIZE);
        nbytes = 0;
        message = {0};

        if ((nbytes = recv(socketfd, receiveBuffer, BUF_SIZE - 1, 0)) < 0)
        {
            std::cerr << "Failed receiving message." << std::endl;
            return;
        }
        receiveBuffer[nbytes] = '\0';

        message.fromString(std::string(receiveBuffer));
        if (message.type == MSG_LOGOUT_TYPE && stop)
        {
            chat.bye();
            return;
        }
        else
            chat.receive(message);

        chat.render();
    }
}

std::string UDPClient::buildMessage(const std::string &username, std::string buffer) const
{
    MessageRequest message = {0};

    if (buffer == ":exit")
        return char(MSG_LOGOUT_TYPE) + username;
    else if (buffer[0] == ':')
    {
        message = {
            MSG_REMOVE_TEXT_TYPE,
            username,
            buffer.substr(1),
        };
    }
    else
    {
        message = {
            MSG_SEND_TEXT_TYPE,
            username,
            buffer,
        };
    }

    return message.toString();
}
