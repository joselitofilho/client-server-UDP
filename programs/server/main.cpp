#include <iostream>
#include "server/UDPServer.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:\n\t" << argv[0] << " <server-port>" << endl;
        exit(EXIT_FAILURE);
    }

    int serverPort = atoi(argv[1]);
    

    UDPServer server("0.0.0.0", serverPort);
    if (!server.init()) {
        exit(EXIT_FAILURE);
    }

    server.start();

    return EXIT_SUCCESS;
}
