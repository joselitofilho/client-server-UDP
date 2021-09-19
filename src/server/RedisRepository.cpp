#include "RedisRepository.h"

RedisRepository::RedisRepository()
{
}

RedisRepository::~RedisRepository()
{
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
