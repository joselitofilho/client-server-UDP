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
            buffer.erase(buffer.begin(), buffer.begin() + 1);
            message.username = buffer;
            break;
        }

        message.type = action;
    }

    return message;
}
