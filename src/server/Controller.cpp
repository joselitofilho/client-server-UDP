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
    Message requestMessage = messageHandler.parseMessage(buffer);
    Message responseMessage = {0};
    std::cout << requestMessage.type << ";" << requestMessage.toString() << std::endl;

    if (requestMessage.type == MSG_INVALID_TYPE)
    {
        return responseMessage;
    }

    loggedUsers.insert_or_assign(requestMessage.username, clientAddr);

    switch (requestMessage.type)
    {
    case MSG_LOGIN_TYPE:
    {
        responseMessage = {
            MSG_SEND_TEXT_TYPE,
            "server",
            requestMessage.username + " is logged in.",
        };
    }
    break;
    case MSG_LOGOUT_TYPE:
    {
        loggedUsers.erase(requestMessage.username);
        responseMessage = {
            MSG_LOGOUT_TYPE,
            "server",
            requestMessage.username + " is logged out.",
        };
    }
    break;
    case MSG_SEND_TEXT_TYPE:
    {
        responseMessage = requestMessage;
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
