#pragma once

#include <iostream>
#include "core/Message.h"

class MessageHandler
{
public:
    MessageHandler();
    ~MessageHandler();

    MessageRequest parseMessage(std::string msg) const;
};
