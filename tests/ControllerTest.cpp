#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "server/Controller.h"
#include "MockRepository.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using namespace jungle;

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
    Message invalidMessageExpected = {0};
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, create(_))
        .WillOnce(Return(0ll));

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(1, (int)loggedUsers.size());
    EXPECT_TRUE(loggedUsers.find("Joselito") != loggedUsers.end());
    EXPECT_EQ(invalidMessageExpected.toString(), message.toString());
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

TEST(ControllerTest, WhenClientDeletesTheirOwnMessage_ReturnsAValidResponse)
{
    struct sockaddr_in clientAddr;
    std::string buffer("Joselito;25");
    buffer.insert(0, 1, char(MSG_REMOVE_TEXT_TYPE));
    std::time_t now = std::time(0);
    Message messageExpected = {
        25ll,
        MSG_REMOVE_TEXT_TYPE,
        now,
        "server",
        "Joselito removed ID=25",
    };
    Messages messages;
    const Message message25 = {25ll, MSG_SEND_TEXT_TYPE, now, "Joselito", "Hi Folks."};
    messages.insert_or_assign(message25.id, message25);
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, all())
        .WillOnce(Return(messages));
    EXPECT_CALL(mockRepository, remove(message25))
        .WillOnce(Return(true));

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(1, (int)loggedUsers.size());
    EXPECT_TRUE(loggedUsers.find("Joselito") != loggedUsers.end());
    EXPECT_EQ(messageExpected.toString(), message.toString());
}

TEST(ControllerTest, WhenClientTriesToDeleteAMessageThatIsNotTheir_ReturnsAInvalidResponse)
{
    struct sockaddr_in clientAddr;
    std::string buffer("Joselito;25");
    buffer.insert(0, 1, char(MSG_REMOVE_TEXT_TYPE));
    std::time_t now = std::time(0);
    Message invalidMessageExpected = {0};
    Messages messages;
    const Message message25 = {25ll, MSG_SEND_TEXT_TYPE, now, "Miguel", "Hi Folks."};
    messages.insert_or_assign(message25.id, message25);
    NiceMock<MockRepository> mockRepository;
    EXPECT_CALL(mockRepository, all())
        .WillOnce(Return(messages));
    EXPECT_CALL(mockRepository, remove(message25)).Times(0);

    Controller controller(&mockRepository);
    auto message = controller.onRequestHandle(buffer, clientAddr);
    message.createdAt = now;

    auto loggedUsers = controller.getLoggedUsers();
    EXPECT_EQ(1, (int)loggedUsers.size());
    EXPECT_TRUE(loggedUsers.find("Joselito") != loggedUsers.end());
    EXPECT_EQ(invalidMessageExpected.toString(), message.toString());
}
