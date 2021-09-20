#pragma once

#include <map>
#include "core/Message.h"

class Repository
{
public:
    Repository() {}
    virtual ~Repository() {}

    virtual std::map<long long, Message> all() = 0;
    virtual bool create(Message &message) = 0;
    virtual void clear() = 0;
    virtual void remove(long long id) = 0;
};
