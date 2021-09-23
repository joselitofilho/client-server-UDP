#pragma once

#include "Repository.h"
#include <hiredis/hiredis.h>

namespace jungle
{
    class RedisRepository : public Repository
    {
    public:
        RedisRepository(const std::string &addr, int port, int capacity = 0);
        RedisRepository(const RedisRepository &other) = delete;
        RedisRepository &operator=(const RedisRepository &) = delete;
        ~RedisRepository();

        bool init();

    public:
        // Repository interface methdos.
        Messages all();
        long long create(Message &message);
        void clear();
        bool remove(const Message &message);

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
}
