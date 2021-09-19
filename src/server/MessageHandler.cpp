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
        int action = (int)buffer[0];
        switch (action)
        {
        case MSG_LOGIN_TYPE:
        case MSG_LOGOUT_TYPE:
            buffer.erase(buffer.begin(), buffer.begin() + 1);
            message.username = buffer;
            break;
        case MSG_SEND_TEXT_TYPE:
            buffer.erase(buffer.begin(), buffer.begin() + 1);
            int separatorPosition = buffer.find_first_of(';');
            message.username = buffer.substr(0, separatorPosition);
            message.text = buffer.substr(separatorPosition + 1);
            break;
        }

        message.type = action;
    }

    return message;
}
