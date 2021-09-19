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
