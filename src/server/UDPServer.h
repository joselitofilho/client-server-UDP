#pragma once

#include <iostream>

class UDPServer
{
public:
    UDPServer(const std::string &addr, int port);
    ~UDPServer();

    bool init();
    void start() const;

private:
    std::string addr;
    int port;
    int socketfd;
};
