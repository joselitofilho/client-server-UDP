#pragma once

#include <iostream>

class UDPClient
{
public:
    UDPClient(const std::string &addr, int serverPort);
    ~UDPClient();

    bool init();
    void login(const std::string &username);
    void start();

private:
    std::string addr;
    std::string username;
    int serverPort;
    int socketfd;
    bool stop;

    void sender();
    void receiver();
};
