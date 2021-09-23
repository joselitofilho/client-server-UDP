#include "RedisRepository.h"
#include <iostream>
#include <cstring>

using namespace jungle;

RedisRepository::RedisRepository(const std::string &addr_, int port_, int capacity_)
    : addr(addr_), port(port_), capacity(capacity_), redisCtx(nullptr)
{
}

RedisRepository::~RedisRepository()
{
    redisFree(redisCtx);
}

bool RedisRepository::init()
{
    redisCtx = redisConnect(addr.c_str(), port);
    if (redisCtx == nullptr || redisCtx->err)
    {
        if (redisCtx)
        {
            std::cerr << "redisConnect::error: " << redisCtx->errstr << std::endl;
            redisFree(redisCtx);
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
    Messages messages;
    Message message;

    trim();

    std::string command("LRANGE messages 0 " + std::to_string(rangeStop()));
    redisReply *reply = (redisReply *)redisCommand(redisCtx, command.c_str());
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

long long RedisRepository::create(Message &message)
{
    long long redisId = 0ll;

    message.id = nextKey();
    message.createdAt = std::time(0);

    redisReply *reply = (redisReply *)redisCommand(redisCtx, "LPUSH messages %s", message.toString().c_str());
    if (reply)
    {
        redisId = reply->integer;
    }
    freeReplyObject(reply);

    return redisId;
}

void RedisRepository::clear()
{
    redisReply *reply = (redisReply *)redisCommand(redisCtx, "DEL messages");
    freeReplyObject(reply);
}

bool RedisRepository::remove(const Message &message)
{
    redisReply *reply = (redisReply *)redisCommand(redisCtx, "LREM messages 1 %s", message.toString().c_str());
    bool isOk = false;
    if (reply) {
        isOk = reply->integer == 1ll;
    }
    freeReplyObject(reply);

    return isOk;
}

long long RedisRepository::nextKey() const
{
    long long nextKey = 0ll;
    redisReply *reply = (redisReply *)redisCommand(redisCtx, "INCR counter");
    if (reply)
        nextKey = reply->integer;
    freeReplyObject(reply);
    return nextKey;
}

bool RedisRepository::ping() const
{
    redisReply *reply = (redisReply *)redisCommand(redisCtx, "PING");
    bool isOK = std::string(reply->str) == "PONG";
    freeReplyObject(reply);

    return isOK;
}

void RedisRepository::trim() const
{
    std::string command("LTRIM messages 0 " + std::to_string(rangeStop()));
    redisReply *reply = (redisReply *)redisCommand(redisCtx, command.c_str());
    freeReplyObject(reply);
}

int RedisRepository::rangeStop() const
{
    int cap = 20;
    if (capacity > 0)
        cap = capacity;
    return cap - 1;
}