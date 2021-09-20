#include "Controller.h"
#include "MessageHandler.h"

Controller::Controller(Repository *repository_ = nullptr)
    : repository(repository_), loggedUsers()
{
}

Controller::~Controller()
{
}

Message Controller::onRequestHandle(const std::string &buffer, struct sockaddr_in clientAddr)
{
    MessageHandler messageHandler;
    MessageRequest requestMessage = messageHandler.parseMessage(buffer);
    
    std::cout << requestMessage.type << ";" << requestMessage.toString() << std::endl;
    if (requestMessage.type == MSG_INVALID_TYPE)
    {
        return {0};
    }

    loggedUsers.insert_or_assign(requestMessage.username, clientAddr);

    Message responseMessage = {0};
    switch (requestMessage.type)
    {
    case MSG_LOGIN_TYPE:
    {
        responseMessage = {
            type: MSG_SEND_TEXT_TYPE,
            from: "server",
            text: requestMessage.username + " is logged in.",
        };
    }
    break;
    case MSG_LOGOUT_TYPE:
    {
        loggedUsers.erase(requestMessage.username);
        responseMessage = {
            type: MSG_LOGOUT_TYPE,
            from: "server",
            text: requestMessage.username + " is logged out.",
        };
    }
    break;
    case MSG_SEND_TEXT_TYPE:
    {
        responseMessage = {
            type: requestMessage.type,
            from: requestMessage.username,
            text: requestMessage.username + " is logged out.",
        };
    }
    break;
    }

    if (repository != nullptr)
    {
        // TODO: Implement
        repository->create(responseMessage);
    }

    return responseMessage;
}
