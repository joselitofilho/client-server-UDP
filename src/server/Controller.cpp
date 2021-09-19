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

    // TODO: repository

    switch (requestMessage.type)
    {
    case MSG_LOGIN_TYPE:
    {
        loggedUsers.insert_or_assign(requestMessage.username, clientAddr);
        responseMessage = {
            MSG_SEND_TEXT_TYPE,
            "server",
            requestMessage.username + " is logged in.",
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