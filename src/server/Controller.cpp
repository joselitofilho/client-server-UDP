#include "Controller.h"

Controller::Controller(Repository *repository_ = nullptr)
    : repository(repository_), loggedUsers()
{
}

Controller::~Controller()
{
}

Messages Controller::getMessages() const
{
    Messages messages;
    if (repository != nullptr)
        messages = repository->all();
    return messages;
}

Message Controller::onRequestHandle(const std::string &buffer, struct sockaddr_in clientAddr)
{
    MessageRequest requestMessage = {0};
    requestMessage.fromString(buffer);

    if (requestMessage.type == MSG_INVALID_TYPE)
        return {0};

    loggedUsers.insert_or_assign(requestMessage.username, clientAddr);

    Message responseMessage = {0};
    switch (requestMessage.type)
    {
    case MSG_LOGIN_TYPE:
    {
        responseMessage = {
            type : MSG_LOGIN_TYPE,
            from : "server",
            text : requestMessage.username + " is logged in.",
        };
    }
    break;
    case MSG_LOGOUT_TYPE:
    {
        loggedUsers.erase(requestMessage.username);
        responseMessage = {
            type : MSG_LOGOUT_TYPE,
            from : "server",
            text : requestMessage.username + " is logged out.",
        };
    }
    break;
    case MSG_SEND_TEXT_TYPE:
    {
        responseMessage = {
            type : requestMessage.type,
            from : requestMessage.username,
            text : requestMessage.text,
        };
    }
    break;
    }

    if (repository != nullptr)
    {
        if (requestMessage.type == MSG_LOGOUT_TYPE && loggedUsers.empty())
        {
            repository->clear();
        }
        else
        {
            long long redisId = repository->create(responseMessage);
            if (!redisId)
            {
                std::cerr << "Failed to save message: " << responseMessage.toString() << std::endl;
                return {0};
            }
        }
    }

    return responseMessage;
}
