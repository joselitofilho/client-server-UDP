#pragma once

#include <iostream>
#include <map>
#include "Controller.h"

class UDPServer
{
public:
    UDPServer(const std::string &addr, int port, Controller &controller);
    ~UDPServer();

    bool init();
    void start();

private:
    std::string addr;
    int port;
    int socketfd;
    Controller &controller;

    void broadcast(const SocketUsers &loggedUsers, const char *buffer) const;
    void sendMessage(const Message &message, struct sockaddr_in clientAddr) const;
    void sendMessages(const Messages &messages, struct sockaddr_in clientAddr) const;
};
