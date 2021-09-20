#include <iostream>
#include <gtest/gtest.h>
#include "server/MessageHandler.h"

TEST(MessageHandlerTest, ParseLoginMessage)
{
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGIN_TYPE));
    MessageHandler messageHandler;

    MessageRequest message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_LOGIN_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageHandlerTest, ParseLogoutMessage)
{
    std::string buffer("Joselito");
    buffer.insert(0, 1, char(MSG_LOGOUT_TYPE));
    MessageHandler messageHandler;

    MessageRequest message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_LOGOUT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageHandlerTest, ParseSendTextMessage)
{
    std::string buffer("Joselito;Hi folks.");
    buffer.insert(0, 1, char(MSG_SEND_TEXT_TYPE));
    MessageHandler messageHandler;

    MessageRequest message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_SEND_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("Hi folks.", message.text);
}

TEST(MessageHandlerTest, ParseRemoveTextMessage)
{
    std::string buffer("Joselito;25");
    buffer.insert(0, 1, char(MSG_REMOVE_TEXT_TYPE));
    MessageHandler messageHandler;

    MessageRequest message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_REMOVE_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("25", message.text);
}

TEST(MessageHandlerTest, WhenTheMessageDoesNotComplyWithTheProtocol_ReturnsAnInvalidMessage)
{
    MessageHandler messageHandler;

    MessageRequest message = messageHandler.parseMessage("Any message.");

    EXPECT_EQ(MSG_INVALID_TYPE, message.type);
    EXPECT_EQ("", message.username);
    EXPECT_EQ("", message.text);
}
