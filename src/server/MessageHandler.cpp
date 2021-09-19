#include "MessageHandler.h"

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

Message MessageHandler::parseMessage(std::string buffer) const
{
    Message message = {0};

    if (buffer.size() > 0)
    {
        message.type = (int)buffer[0];
        buffer.erase(buffer.begin(), buffer.begin() + 1);

        switch (message.type)
        {
        case MSG_LOGIN_TYPE:
        case MSG_LOGOUT_TYPE:
            message.username = buffer;
            break;
        case MSG_SEND_TEXT_TYPE:
        case MSG_REMOVE_TEXT_TYPE:
            int separatorPosition = buffer.find_first_of(';');
            message.username = buffer.substr(0, separatorPosition);
            message.text = buffer.substr(separatorPosition + 1);
            break;
        }
    }

    return message;
}
