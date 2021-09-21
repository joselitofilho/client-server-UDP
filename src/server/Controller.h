#pragma once

#include <arpa/inet.h>
#include <map>
#include <string.h>
#include "core/Message.h"
#include "Repository.h"

typedef std::map<std::string, struct sockaddr_in> SocketUsers;

class Controller
{
public:
    Controller(Repository *repository);
    ~Controller();

    const SocketUsers &getLoggedUsers() const { return loggedUsers; };
    Messages getMessages() const;

    Message onRequestHandle(const std::string &buffer, struct sockaddr_in clientAddr);

private:
    Repository *repository;
    SocketUsers loggedUsers;
};
