#pragma once

#include <gmock/gmock.h>

class MockRepository : public Repository
{
public:
    MOCK_METHOD0(all, Messages());
    MOCK_METHOD1(create, long long(Message &message));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD1(remove, bool(const Message &message));
};
