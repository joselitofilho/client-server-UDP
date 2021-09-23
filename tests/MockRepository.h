#pragma once

#include <gmock/gmock.h>

class MockRepository : public jungle::Repository
{
public:
    MOCK_METHOD0(all, jungle::Messages());
    MOCK_METHOD1(create, long long(jungle::Message &message));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD1(remove, bool(const jungle::Message &message));
};
