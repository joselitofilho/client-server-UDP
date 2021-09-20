#pragma once

#include <string>
#include <ctime>
#include <sstream>

#define MSG_INVALID_TYPE 0
#define MSG_LOGIN_TYPE 1
#define MSG_LOGOUT_TYPE 2
#define MSG_SEND_TEXT_TYPE 3
#define MSG_REMOVE_TEXT_TYPE 4

struct MessageRequest
{
    int type = MSG_INVALID_TYPE;
    std::string username = "";
    std::string text = "";

    std::string toString() const
    {
        return char(type) + username + ";" + text;
    }
};

struct Message
{
    long long id = 0ll;
    int type = MSG_INVALID_TYPE;
    std::time_t createdAt = std::time(0);
    std::string from = "";
    std::string text = "";

    std::string toString() const
    {
        std::stringstream ss;
        ss << std::to_string(id) << ";"
           << std::to_string(type) << ";"
           << createdAt << ";"
           << from << ";"
           << text;
        return ss.str();
    }
};
