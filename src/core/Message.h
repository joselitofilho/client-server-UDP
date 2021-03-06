#pragma once

#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include <map>

namespace jungle
{
    struct Message;
    typedef std::map<long long, Message> Messages;

    enum
    {
        MSG_INVALID_TYPE,
        MSG_LOGIN_TYPE,
        MSG_LOGOUT_TYPE,
        MSG_SEND_TEXT_TYPE,
        MSG_REMOVE_TEXT_TYPE
    };

    struct MessageRequest
    {
        int type{MSG_INVALID_TYPE};
        std::string username{""};
        std::string text{""};

        std::string toString() const
        {
            if (type == MSG_INVALID_TYPE)
            {
                return "";
            }
            return char(type) + username + ";" + text;
        }

        void fromString(std::string buffer)
        {
            if (buffer.size() > 0)
            {
                type = (int)buffer[0];
                buffer.erase(buffer.begin(), buffer.begin() + 1);

                switch (type)
                {
                case MSG_LOGIN_TYPE:
                case MSG_LOGOUT_TYPE:
                {
                    username = buffer;
                }
                break;
                case MSG_SEND_TEXT_TYPE:
                case MSG_REMOVE_TEXT_TYPE:
                {
                    int separatorPosition = buffer.find_first_of(';');
                    username = buffer.substr(0, separatorPosition);
                    text = buffer.substr(separatorPosition + 1);
                }
                break;
                default:
                {
                    type = MSG_INVALID_TYPE;
                }
                break;
                }
            }
        }
    };

    struct Message
    {
        long long id{0};
        int type{MSG_INVALID_TYPE};
        std::time_t createdAt{0};
        std::string from{""};
        std::string text{""};

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

        void fromString(std::string buffer)
        {
            int start = 0, end = 0;

            end = buffer.find(';', start);
            id = std::stoll(buffer.substr(start, end - start));
            start = end + 1;

            end = buffer.find(';', start);
            type = atoi(buffer.substr(start, end - start).c_str());
            start = end + 1;

            end = buffer.find(';', start);
            std::stringstream ss;
            ss << buffer.substr(start, end - start);
            ss >> createdAt;
            ss.str("");
            start = end + 1;

            end = buffer.find(';', start);
            from = buffer.substr(start, end - start);
            start = end + 1;

            text = buffer.substr(start);
        }

        void fromMessageRequest(const MessageRequest &requestMessage)
        {
            switch (requestMessage.type)
            {
            case MSG_LOGIN_TYPE:
            {
                type = MSG_LOGIN_TYPE;
                from = "server";
                text = requestMessage.username + " is logged in.";
            }
            break;
            case MSG_LOGOUT_TYPE:
            {
                type = MSG_LOGOUT_TYPE;
                from = "server";
                text = requestMessage.username + " is logged out.";
            };

            break;
            case MSG_SEND_TEXT_TYPE:
            {
                type = requestMessage.type;
                from = requestMessage.username;
                text = requestMessage.text;
            }
            break;
            case MSG_REMOVE_TEXT_TYPE:
            {
                id = std::stoll(requestMessage.text);
                type = MSG_REMOVE_TEXT_TYPE;
                from = "server";
                text = requestMessage.username + " removed ID=" + requestMessage.text;
            }
            break;
            }
        }

        bool operator==(const Message &rh) const
        {
            return id == rh.id && type == rh.type && createdAt == rh.createdAt && from == rh.from && text == rh.text;
        }
    };
}
