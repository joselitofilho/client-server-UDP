#pragma once

#include "Repository.h"

class RedisRepository : public Repository
{
public:
    RedisRepository();
    ~RedisRepository();

    std::map<unsigned int, Message> all();
    unsigned int create(const Message &message);
    void clear();
    void remove(unsigned int id);
};
