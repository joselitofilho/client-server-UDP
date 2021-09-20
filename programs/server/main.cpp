#include <iostream>
#include "server/UDPServer.h"
#include "server/Controller.h"
#include "server/RedisRepository.h"

using namespace std;

int main(int argc, char *argv[])
{
    int serverPort;

    if (argc < 2)
    {
        cout << "Usage:\n\t" << argv[0] << " <server-port>" << endl;
        exit(EXIT_FAILURE);
    }
    serverPort = atoi(argv[1]);

    RedisRepository redisRepository("127.0.0.1", 6379);
    if (!redisRepository.init())
    {
        exit(EXIT_FAILURE);
    }

    Controller controller(&redisRepository);

    UDPServer server("0.0.0.0", serverPort, controller);
    if (!server.init())
    {
        exit(EXIT_FAILURE);
    }

    server.start();

    return EXIT_SUCCESS;
}
