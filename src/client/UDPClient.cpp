#include "client/UDPClient.h"
#include "core/constants.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

UDPClient::UDPClient(const std::string &addr_, int serverPort_)
    : addr(addr_), serverPort(serverPort_), socketfd(-1), stop(false)
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
    std::string cmdLoginBuffer;
    cmdLoginBuffer = char(1) + username;
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
    char sendBuffer[BUF_SIZE];

    while (!stop)
    {
        bzero(sendBuffer, BUF_SIZE);
        fgets(sendBuffer, BUF_SIZE, stdin);

        if (send(socketfd, sendBuffer, strlen(sendBuffer), 0) < 0)
        {
            std::cerr << "Failed to send message." << std::endl;
            close(socketfd);
            stop = true;
        }
    }
}

void UDPClient::receiver()
{
    char receiveBuffer[BUF_SIZE];
    int nbytes;

    while (!stop)
    {
        bzero(receiveBuffer, BUF_SIZE);
        nbytes = 0;

        if ((nbytes = recv(socketfd, receiveBuffer, BUF_SIZE - 1, 0)) < 0)
        {
            std::cerr << "Failed receiving message." << std::endl;
            close(socketfd);
            stop = true;
        }

        receiveBuffer[nbytes] = '\0';
        std::cout << receiveBuffer << std::endl;

        // TODO: Implement
    }
}
