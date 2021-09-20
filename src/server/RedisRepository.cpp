#include "RedisRepository.h"
#include <iostream>
#include <cstring>

RedisRepository::RedisRepository(const std::string &addr_, int port_)
    : addr(addr_), port(port_), redisCtx(nullptr)
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

    redisReply *reply = (redisReply *)redisCommand(redisCtx, "PING");
    std::cout << "RedisRepository::PING:" << reply->str << std::endl;
    bool isOK = std::string(reply->str) == "PONG";
    freeReplyObject(reply);

    return isOK;
}

std::map<long long, Message> RedisRepository::all()
{
    // TODO: Implements
    return std::map<long long, Message>();
}

bool RedisRepository::create(Message &message)
{
    bool isOk = false;

    message.createdAt = std::time(0);

    std::stringstream ssCommand;
    ssCommand << std::to_string(message.type) << ";"
              << message.createdAt << ";"
              << message.from << ";"
              << message.text;
    redisReply *reply = (redisReply *)redisCommand(redisCtx, "LPUSH messages %s", ssCommand.str().c_str());
    if (reply)
    {
        message.id = reply->integer;
        isOk = message.id > 0ll;
    }
    freeReplyObject(reply);

    return isOk;
}

void RedisRepository::clear()
{
    // TODO: Implements
}

void RedisRepository::remove(long long id)
{
    // TODO: Implements
    std::cout << "RedisRepository::remove::" << id << std::endl;
}
