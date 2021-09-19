#pragma once

#include <iostream>
#include <map>
#include "core/Message.h"

class Repository
{
public:
    Repository() {}
    virtual ~Repository() {}

    virtual std::map<unsigned int, Message> all() = 0;
    virtual unsigned int create(const Message &message) = 0;
    virtual void clear() = 0;
    virtual void remove(unsigned int id) = 0;
};
