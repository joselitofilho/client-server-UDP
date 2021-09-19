#include <string>

#define MSG_LOGIN_TYPE 1
#define MSG_LOGOUT_TYPE 2
#define MSG_SEND_TEXT_TYPE 3
#define MSG_REMOVE_TEXT_TYPE 4

struct Message
{
    int type = 0;
    std::string username = "";
    std::string text = "";
};
