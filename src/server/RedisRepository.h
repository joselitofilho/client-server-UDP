#pragma once

#include "Repository.h"
#include "core/constants.h"
#include <hiredis/hiredis.h>

namespace jungle
{
    class RedisRepository : public Repository
    {
    public:
        RedisRepository(const std::string &t_addr, int t_port, int t_capacity = REDIS_UNLIMITED_CAPACITY);
        RedisRepository(const RedisRepository &) = delete;
        RedisRepository &operator=(const RedisRepository &) = delete;
        ~RedisRepository();

        bool init();

    public:
        // Repository interface methods.
        Messages all();
        long long create(Message &t_message);
        void clear();
        bool remove(const Message &t_message);

    private:
        long long nextKey() const;
        bool ping() const;
        int rangeStop() const;
        void trim() const;

    private:
        const std::string m_addr{DEFAULT_SERVER_ADDR};
        const int m_port{DEFAULT_SERVER_PORT};
        const int m_capacity{REDIS_UNLIMITED_CAPACITY};
        redisContext *m_redisCtx{nullptr};
    };
}
