#pragma once

#include <arpa/inet.h>
#include <map>
#include <string.h>
#include "core/Message.h"
#include "Repository.h"

namespace jungle
{
    typedef std::map<std::string, struct sockaddr_in> SocketUsers;

    class Controller
    {
    public:
        Controller(Repository *t_repository = nullptr);
        Controller(const Controller &) = delete;
        Controller &operator=(const Controller &) = delete;
        ~Controller();

        const SocketUsers &getLoggedUsers() const { return m_loggedUsers; };

        Messages allMessages() const;
        Message onRequestHandle(const std::string &t_buffer, struct sockaddr_in t_clientAddr);

    private:
        Repository *m_repository{nullptr};
        SocketUsers m_loggedUsers{};
    };
}
