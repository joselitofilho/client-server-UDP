#pragma once

#include "Repository.h"
#include <hiredis/hiredis.h>

class RedisRepository : public Repository
{
public:
    RedisRepository(const std::string &addr, int port);
    ~RedisRepository();

    bool init();

    std::map<unsigned int, Message> all();
    unsigned int create(const Message &message);
    void clear();
    void remove(unsigned int id);

private:
    const std::string addr;
    int port;
    redisContext *redisCtx;
};
