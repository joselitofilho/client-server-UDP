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
    responseMessage.fromMessageRequest(requestMessage);

    if (responseMessage.type == MSG_LOGOUT_TYPE)
        loggedUsers.erase(requestMessage.username);

    if (repository != nullptr)
    {
        if (requestMessage.type == MSG_LOGOUT_TYPE && loggedUsers.empty())
            repository->clear();
        else if (requestMessage.type == MSG_REMOVE_TEXT_TYPE)
        {
            auto messages = getMessages();
            auto message = messages[responseMessage.id];
            if (!(message.from == requestMessage.username && repository->remove(message)))
                return {0};
        }
        else
        {
            if (!repository->create(responseMessage))
            {
                std::cerr << "Failed to save message: " << responseMessage.toString() << std::endl;
                return {0};
            }
        }
    }

    return responseMessage;
}
