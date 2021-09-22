#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "server/Controller.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

class MockRepository : public Repository
{
public:
    MOCK_METHOD0(all, Messages());
    MOCK_METHOD1(create, long long(Message &message));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD1(remove, bool(const Message &message));
};

TEST(ControllerTest, OnRequestHandleForLoginBuffer)
{
    struct sockaddr_in clientAddr;
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGIN_TYPE));
    std::time_t now = std::time(0);
    Message messageExpected = {
        0ll,
        MSG_LOGIN_TYPE,
        now,
        "server",
        "Joselito is logged in.",
    };
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, create(_))
        .WillOnce(Return(1ll));

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(1, (int)loggedUsers.size());
    EXPECT_TRUE(loggedUsers.find("Joselito") != loggedUsers.end());
    EXPECT_EQ(messageExpected.toString(), message.toString());
}

TEST(ControllerTest, OnRequestHandleForLoginBuffer_WhenRepositoryCreateFailed_ReturnsAnInvalidMessage)
{
    struct sockaddr_in clientAddr;
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGIN_TYPE));
    std::time_t now = std::time(0);
    Message messageExpected = {0};
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, create(_))
        .WillOnce(Return(0ll));

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(1, (int)loggedUsers.size());
    EXPECT_TRUE(loggedUsers.find("Joselito") != loggedUsers.end());
    EXPECT_EQ(messageExpected.toString(), message.toString());
}

TEST(ControllerTest, WhenAllClientsDisconnectTheDBIsFlushed)
{
    struct sockaddr_in clientAddr;
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGOUT_TYPE));
    std::time_t now = std::time(0);
    Message messageExpected = {
        0ll,
        MSG_LOGOUT_TYPE,
        now,
        "server",
        "Joselito is logged out.",
    };
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, clear()).Times(1);

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(0, (int)loggedUsers.size());
    EXPECT_EQ(messageExpected.toString(), message.toString());
}
