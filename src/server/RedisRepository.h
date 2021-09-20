#pragma once

#include "Repository.h"
#include <hiredis/hiredis.h>

class RedisRepository : public Repository
{
public:
    RedisRepository(const std::string &addr, int port);
    ~RedisRepository();

    bool init();

    std::map<long long, Message> all();
    bool create(Message &message);
    void clear();
    void remove(long long id);

private:
    const std::string addr;
    int port;
    redisContext *redisCtx;
};
