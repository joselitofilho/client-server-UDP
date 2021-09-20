#include "RedisRepository.h"

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

std::map<unsigned int, Message> RedisRepository::all()
{
    // TODO: Implements
    return std::map<unsigned int, Message>();
}

unsigned int RedisRepository::create(const Message &message)
{
    // TODO: Implements
    std::cout << "RedisRepository::create::" << message.toString() << std::endl;
    return 0;
}

void RedisRepository::clear()
{
    // TODO: Implements
}

void RedisRepository::remove(unsigned int id)
{
    // TODO: Implements
    std::cout << "RedisRepository::remove::" << id << std::endl;
}
