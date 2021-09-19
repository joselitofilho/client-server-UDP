#include "Controller.h"
#include "MessageHandler.h"

Controller::Controller()
    : loggedUsers()
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
    std::cout << "From: " << requestMessage.username << " - Message: " << requestMessage.text << std::endl;

    if (requestMessage.type == MSG_INVALID_TYPE)
    {
        return responseMessage;
    }

    loggedUsers.insert_or_assign(requestMessage.username, clientAddr);

    // TODO: repository

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

    return responseMessage;
}
