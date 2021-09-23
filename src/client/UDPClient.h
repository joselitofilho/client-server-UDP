#pragma once

#include <string>
#include "Chat.h"

namespace jungle
{
    class UDPClient
    {
    public:
        UDPClient(const std::string &addr, int serverPort);
        ~UDPClient();

        bool init();
        void login(const std::string &username);
        void start();

    private:
        void sender();
        void receiver();
        std::string buildMessage(const std::string &username, std::string buffer) const;

    private:
        std::string addr;
        std::string username;
        int serverPort;
        int socketfd;
        bool stop;
        Chat chat;
    };
}
