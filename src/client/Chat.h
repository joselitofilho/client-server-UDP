#pragma once

#include "core/Message.h"

class Chat
{
public:
    Chat();
    ~Chat();

    void bye() const;
    void clear() const;
    void receive(const Message &message);
    void render() const;

private:
    Messages messages;

    void welcome() const;
};
