#include <string>

#define MSG_INVALID_TYPE 0
#define MSG_LOGIN_TYPE 1
#define MSG_LOGOUT_TYPE 2
#define MSG_SEND_TEXT_TYPE 3
#define MSG_REMOVE_TEXT_TYPE 4

struct Message
{
    int type = MSG_INVALID_TYPE;
    std::string username = "";
    std::string text = "";
};
