#include <iostream>
#include <gtest/gtest.h>
#include "server/MessageHandler.h"

TEST(MessageHandlerTest, ParseLoginMessage)
{
    std::string buffer = "_Joselito";
    buffer[0] = MSG_LOGIN_TYPE;
    MessageHandler messageHandler;

    Message message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_LOGIN_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageHandlerTest, ParseLogoutMessage)
{
    std::string buffer = "_Joselito";
    buffer[0] = MSG_LOGOUT_TYPE;
    MessageHandler messageHandler;

    Message message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_LOGOUT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("", message.text);
}

TEST(MessageHandlerTest, ParseSendTextMessage)
{
    std::string buffer = "_Joselito;Hi folks.";
    buffer[0] = MSG_SEND_TEXT_TYPE;
    MessageHandler messageHandler;

    Message message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_SEND_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("Hi folks.", message.text);
}

TEST(MessageHandlerTest, ParseRemoveTextMessage)
{
    std::string buffer = "_Joselito;25";
    buffer[0] = MSG_REMOVE_TEXT_TYPE;
    MessageHandler messageHandler;

    Message message = messageHandler.parseMessage(buffer);

    EXPECT_EQ(MSG_REMOVE_TEXT_TYPE, message.type);
    EXPECT_EQ("Joselito", message.username);
    EXPECT_EQ("25", message.text);
}

TEST(MessageHandlerTest, when_the_message_does_not_comply_with_the_protocol_returns_an_invalid_message)
{
    MessageHandler messageHandler;

    Message message = messageHandler.parseMessage("Any message.");

    EXPECT_EQ(MSG_INVALID_TYPE, message.type);
    EXPECT_EQ("", message.username);
    EXPECT_EQ("", message.text);
}
