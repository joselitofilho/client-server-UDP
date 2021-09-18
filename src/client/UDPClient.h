#pragma once

#include <iostream>

class UDPClient
{
public:
    UDPClient(const std::string &addr, int serverPort);
    ~UDPClient();

    bool init();
    void start();

private:
    std::string addr;
    int serverPort;
    int socketfd;
    bool stop;

    void sender();
    void receiver();
};
