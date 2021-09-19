#pragma once

#include <iostream>
#include "core/Message.h"

class MessageHandler
{
public:
    MessageHandler();
    ~MessageHandler();

    Message parseMessage(std::string msg) const;
};
