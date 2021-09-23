#include "Chat.h"

using namespace jungle;

Chat::Chat()
    : m_serverIsOn{false}, m_messages{}
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
    {
        m_messages.erase(message.id);
    }
    else
    {
        m_messages.insert_or_assign(message.id, message);
    }
}

void Chat::render() const
{
    clear();

    std::cout << "Welcome to the Jungle! Your favorite UDP chat.";
    if (!m_serverIsOn)
    {
        std::cout << "\n[Server is OFF]";
    }
    std::cout << "\n\nChat:\n";

    auto it = m_messages.begin();
    while (it != m_messages.end())
    {
        std::cout << it->second.id << " - " << it->second.from << ": " << it->second.text << std::endl;
        ++it;
    }

    std::cout << "\nType what you want or:\n"
              << "\t:exit - Logout.\n"
              << "\t:ID   - Remove a message by ID.\n";
    std::cout << "-> " << std::flush;
}
