#pragma once

#include <iostream>
#include <map>
#include "core/constants.h"
#include "Controller.h"

namespace jungle
{
    class UDPServer
    {
    public:
        UDPServer(const std::string &t_addr, int t_port, Controller &t_controller);
        UDPServer(const UDPServer &) = delete;
        UDPServer &operator=(const UDPServer &) = delete;
        ~UDPServer();

        bool init();
        void start() const;

    private:
        void broadcast(const SocketUsers &t_users, const char *t_buffer) const;
        void sendMessage(const char *t_buffer, struct sockaddr_in t_clientAddr) const;
        void sendMessages(const Messages &t_messages, struct sockaddr_in t_clientAddr) const;

    private:
        const std::string m_addr{DEFAULT_SERVER_ADDR};
        const int m_port{DEFAULT_SERVER_PORT};
        int m_socketfd{-1};
        Controller &m_controller;
    };
}
