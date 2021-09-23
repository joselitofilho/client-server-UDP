#include "Controller.h"

using namespace jungle;

Controller::Controller(Repository *t_repository)
    : m_repository{t_repository}, m_loggedUsers{}
{
}

Controller::~Controller()
{
}

Messages Controller::allMessages() const
{
    Messages messages;
    if (m_repository != nullptr)
    {
        messages = m_repository->all();
    }
    return messages;
}

Message Controller::onRequestHandle(const std::string &t_buffer, struct sockaddr_in t_clientAddr)
{
    MessageRequest requestMessage{0};
    requestMessage.fromString(t_buffer);

    if (requestMessage.type == MSG_INVALID_TYPE)
    {
        return {0};
    }

    m_loggedUsers.insert_or_assign(requestMessage.username, t_clientAddr);

    Message responseMessage{0};
    responseMessage.fromMessageRequest(requestMessage);

    if (responseMessage.type == MSG_LOGOUT_TYPE)
    {
        m_loggedUsers.erase(requestMessage.username);
    }

    if (m_repository != nullptr)
    {
        if (requestMessage.type == MSG_LOGOUT_TYPE && m_loggedUsers.empty())
        {
            m_repository->clear();
        }
        else if (requestMessage.type == MSG_REMOVE_TEXT_TYPE)
        {
            auto messages = allMessages();
            auto message = messages[responseMessage.id];
            auto isRemoved = message.from == requestMessage.username && m_repository->remove(message);
            if (!isRemoved)
            {
                return {0};
            }
        }
        else
        {
            if (!m_repository->create(responseMessage))
            {
                std::cerr << "Failed to save message: " << responseMessage.toString() << std::endl;
                return {0};
            }
        }
    }

    return responseMessage;
}
