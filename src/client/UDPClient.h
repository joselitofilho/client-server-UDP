#pragma once

#include <string>
#include "core/constants.h"
#include "Chat.h"

namespace jungle
{
    class UDPClient
    {
    public:
        UDPClient(const std::string &t_addr, int t_serverPort);
        UDPClient(const UDPClient &) = delete;
        UDPClient &operator=(const UDPClient &) = delete;
        ~UDPClient();

        bool init();
        void login(const std::string &t_username);
        void start();

    private:
        void sender();
        void receiver();
        std::string buildMessage(const std::string &t_username, std::string t_buffer) const;

    private:
        const std::string m_addr{DEFAULT_SERVER_ADDR};
        std::string m_username{""};
        const int m_serverPort{DEFAULT_SERVER_PORT};
        int m_socketfd{-1};
        bool m_stop{false};
        Chat m_chat{};
    };
}
