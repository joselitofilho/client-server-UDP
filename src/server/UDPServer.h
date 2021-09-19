#include <iostream>
#include <map>
#include <unistd.h>


class UDPServer
{
public:
    UDPServer(const std::string &addr, int port);
    ~UDPServer();

    void broadcast(std::map<std::string, struct sockaddr_in> &loggedUsers, const char *message) const;
    bool init();
    void start();

private:
    std::string addr;
    int port;
    int socketfd;
};
