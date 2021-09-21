#pragma once

#include "core/Message.h"

class Repository
{
public:
    Repository() {}
    virtual ~Repository() {}

    virtual Messages all() = 0;
    virtual long long create(Message &message) = 0;
    virtual void clear() = 0;
    virtual bool remove(const Message &message) = 0;
};
