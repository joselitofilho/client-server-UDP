#include "RedisRepository.h"
#include <iostream>
#include <cstring>

using namespace jungle;

RedisRepository::RedisRepository(const std::string &t_addr, int t_port, int t_capacity)
    : m_addr{t_addr}, m_port{t_port}, m_capacity{t_capacity}, m_redisCtx{nullptr}
{
}

RedisRepository::~RedisRepository()
{
    redisFree(m_redisCtx);
    if (m_redisCtx != nullptr)
    {
        delete m_redisCtx;
        m_redisCtx = nullptr;
    }
}

bool RedisRepository::init()
{
    m_redisCtx = redisConnect(m_addr.c_str(), m_port);
    if (m_redisCtx == nullptr || m_redisCtx->err)
    {
        if (m_redisCtx)
        {
            std::cerr << "redisConnect::error: " << m_redisCtx->errstr << std::endl;
            redisFree(m_redisCtx);
            return false;
        }
        else
        {
            std::cerr << "Can't allocate redis context\n";
            return false;
        }
    }

    return ping();
}

Messages RedisRepository::all()
{
    Messages messages{};
    Message message;

    trim();

    auto command("LRANGE messages 0 " + std::to_string(rangeStop()));
    auto *reply = (redisReply *)redisCommand(m_redisCtx, command.c_str());
    if (reply)
    {
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            for (size_t i = 0; i < reply->elements; ++i)
            {
                message = {0};
                message.fromString(reply->element[i]->str);
                messages.insert_or_assign(message.id, message);
            }
        }
    }
    freeReplyObject(reply);

    return messages;
}

long long RedisRepository::create(Message &t_message)
{
    auto redisID = 0ll;

    t_message.id = nextKey();
    t_message.createdAt = std::time(0);

    auto *reply = (redisReply *)redisCommand(m_redisCtx, "LPUSH messages %s", t_message.toString().c_str());
    if (reply)
    {
        redisID = reply->integer;
    }
    freeReplyObject(reply);

    return redisID;
}

void RedisRepository::clear()
{
    auto *reply = (redisReply *)redisCommand(m_redisCtx, "DEL messages");
    freeReplyObject(reply);
}

bool RedisRepository::remove(const Message &t_message)
{
    auto isOk = false;

    auto *reply = (redisReply *)redisCommand(m_redisCtx, "LREM messages 1 %s", t_message.toString().c_str());
    if (reply)
    {
        isOk = reply->integer == 1ll;
    }
    freeReplyObject(reply);

    return isOk;
}

long long RedisRepository::nextKey() const
{
    auto nextKey = 0ll;

    auto *reply = (redisReply *)redisCommand(m_redisCtx, "INCR counter");
    if (reply)
    {
        nextKey = reply->integer;
    }
    freeReplyObject(reply);

    return nextKey;
}

bool RedisRepository::ping() const
{
    auto isOK = false;

    auto *reply = (redisReply *)redisCommand(m_redisCtx, "PING");
    if (reply)
    {
        isOK = strcmp(reply->str, "PONG");
    }
    freeReplyObject(reply);

    return isOK;
}

void RedisRepository::trim() const
{
    std::string command("LTRIM messages 0 " + std::to_string(rangeStop()));
    auto *reply = (redisReply *)redisCommand(m_redisCtx, command.c_str());
    freeReplyObject(reply);
}

int RedisRepository::rangeStop() const
{
    int cap = REDIS_MAX_CAPACITY;
    if (m_capacity > 0)
    {
        cap = m_capacity;
    }
    return cap - 1;
}