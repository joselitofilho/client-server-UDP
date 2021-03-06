#pragma once

#include "core/Message.h"

namespace jungle
{
    class Repository
    {
    public:
        Repository() {}
        virtual ~Repository() {}

        virtual Messages all() = 0;
        virtual long long create(Message &t_message) = 0;
        virtual void clear() = 0;
        virtual bool remove(const Message &t_message) = 0;
    };
}