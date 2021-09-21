#include <iostream>
#include <gtest/gtest.h>
#include "core/Message.h"

TEST(MessageRequestTest, ParseLoginMessage)
{
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGIN_TYPE));
    MessageRequest message = {0};

    message.fromString(buffer);

    EXPECT_EQ(MSG_LOGIN_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageRequestTest, ParseLogoutMessage)
{
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGOUT_TYPE));
    MessageRequest message = {0};
    
    message.fromString(buffer);

    EXPECT_EQ(MSG_LOGOUT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageRequestTest, ParseSendTextMessage)
{
    std::string buffer("Joselito;Hi folks.");
    buffer.insert(0, 1, char(MSG_SEND_TEXT_TYPE));
    MessageRequest message = {0};
    
    message.fromString(buffer);

    EXPECT_EQ(MSG_SEND_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("Hi folks.", message.text);
}

TEST(MessageRequestTest, ParseRemoveTextMessage)
{
    std::string buffer("Joselito;25");
    buffer.insert(0, 1, char(MSG_REMOVE_TEXT_TYPE));
    MessageRequest message = {0};
    
    message.fromString(buffer);

    EXPECT_EQ(MSG_REMOVE_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("25", message.text);
}

TEST(MessageRequestTest, WhenTheMessageDoesNotComplyWithTheProtocol_ReturnsAnInvalidMessage)
{
    MessageRequest message = {0};
    
    message.fromString("Any message.");

    EXPECT_EQ(MSG_INVALID_TYPE, message.type);
    EXPECT_EQ("", message.username);
    EXPECT_EQ("", message.text);
}
