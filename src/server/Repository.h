#pragma once

#include <map>
#include "core/Message.h"

class Repository
{
public:
    Repository() {}
    virtual ~Repository() {}

    virtual Messages all() = 0;
    virtual long long create(Message &message) = 0;
    virtual void clear() = 0;
    virtual void remove(long long id) = 0;
};
