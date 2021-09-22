#pragma once

#include "core/Message.h"

class Chat
{
public:
    Chat();
    ~Chat();

    void setServerIsOn(bool value) { serverIsOn = value; };

    void bye() const;
    void clear() const;
    void receive(const Message &message);
    void render() const;

private:
    bool serverIsOn;
    Messages messages;

    void welcome() const;
};
