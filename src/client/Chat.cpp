#include "Chat.h"

Chat::Chat()
    : messages()
{
}

Chat::~Chat()
{
}

void Chat::bye() const
{
    std::cout << "Have a nice day. Bye! :D\n";
}

void Chat::clear() const
{
    // #ifdef WINDOWS
    //     std::system("cls");
    // #else
    //     std::system("clear");
    // #endif
    std::cout << std::string(100, '\n'); // Safe mode.
}

void Chat::receive(const Message &message)
{
    if (message.type == MSG_REMOVE_TEXT_TYPE)
        messages.erase(message.id);
    else
        messages.insert_or_assign(message.id, message);
}

void Chat::render() const
{
    clear();

    std::cout << "Welcome to the Jungle! Your favorite UDP chat.\n\n";
    std::cout << "Chat:\n";

    Messages::const_iterator it;
    it = messages.begin();
    while (it != messages.end())
    {
        std::cout << it->second.id << " - " << it->second.from << ": " << it->second.text << std::endl;
        ++it;
    }

    std::cout << "\nType what you want or:\n"
              << "\t:exit - Logout.\n"
              << "\t:ID   - Remove a message by ID.\n";
    std::cout << "-> " << std::flush;
}
