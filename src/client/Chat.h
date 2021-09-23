#pragma once

#include "core/Message.h"

namespace jungle
{
    class Chat
    {
    public:
        Chat();
        Chat(const Chat &) = delete;
        Chat &operator=(const Chat &) = delete;
        ~Chat();

        void setServerIsOn(bool t_value) { m_serverIsOn = t_value; };

        void bye() const;
        void clear() const;
        void receive(const Message &t_message);
        void render() const;

    private:
        void welcome() const;

    private:
        bool m_serverIsOn{false};
        Messages m_messages{};
    };
}
