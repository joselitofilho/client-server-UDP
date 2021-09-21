#pragma once

#include "Repository.h"
#include <hiredis/hiredis.h>

class RedisRepository : public Repository
{
public:
    RedisRepository(const std::string &addr, int port, int capacity = 0);
    ~RedisRepository();

    bool init();

public:
    /**
     * Repository interface
     */
    Messages all();
    long long create(Message &message);
    void clear();
    void remove(long long id);

private:
    long long nextKey() const;
    bool ping() const;
    int rangeStop() const;
    void trim() const;

private:
    const std::string addr;
    int port;
    int capacity;
    redisContext *redisCtx;
};
