#include <iostream>
#include <gtest/gtest.h>
#include "core/Message.h"

using namespace jungle;

TEST(MessageTest, ParseLoginMessageBuffer)
{
    auto buffer("1;1;1632417387;server;Ze is logged in.");
    Message expectedMessage{1, 1, 1632417387, "server", "Ze is logged in."};

    Message message{0};
    message.fromString(buffer);

    EXPECT_EQ(expectedMessage, message);
}

TEST(MessageTest, ParseLogoutMessageBuffer)
{
    auto buffer("1;2;1632417387;server;Ze is logged out.");
    Message expectedMessage{1, 2, 1632417387, "server", "Ze is logged out."};

    Message message{0};
    message.fromString(buffer);

    EXPECT_EQ(expectedMessage, message);
}

TEST(MessageTest, ParseSendTextMessageBuffer)
{
    auto buffer("1;3;1632417387;Joselito;Hi Folks.");
    Message expectedMessage{1, 3, 1632417387, "Joselito", "Hi Folks."};

    Message message{0};
    message.fromString(buffer);

    EXPECT_EQ(expectedMessage, message);
}

TEST(MessageTest, ParseRemoveTextMessageBuffer)
{
    auto buffer("1;4;1632417387;Joselito;25");
    Message expectedMessage{1, 4, 1632417387, "Joselito", "25"};

    Message message{0};
    message.fromString(buffer);

    EXPECT_EQ(expectedMessage, message);
}
