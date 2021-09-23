#pragma once

#include <iostream>
#include <map>
#include "Controller.h"

namespace jungle
{
    class UDPServer
    {
    public:
        UDPServer(const std::string &addr, int port, Controller &controller);
        UDPServer(const UDPServer &other) = delete;
        UDPServer &operator=(const UDPServer &) = delete;
        ~UDPServer();

        bool init();
        void start();

    private:
        void broadcast(const SocketUsers &loggedUsers, const char *buffer) const;
        void sendMessage(const char *buffer, struct sockaddr_in clientAddr) const;
        void sendMessages(const Messages &messages, struct sockaddr_in clientAddr) const;

    private:
        std::string addr;
        int port;
        int socketfd;
        Controller &controller;
    };
}
