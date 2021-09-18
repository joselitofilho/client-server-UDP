#include <iostream>
#include <string.h>
#include "core/constants.h"
#include "UDPClient.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage:\n\t" << argv[0] << " <server-port> <user-name>" << endl;
        exit(EXIT_FAILURE);
    }

    int serverPort = atoi(argv[1]);
    char username[USERNAME_LEN];
    strncpy(username, argv[2], USERNAME_LEN);

    UDPClient client("127.0.0.1", serverPort);
    if (!client.init()) {
        exit(EXIT_FAILURE);
    }

    client.start();

    return EXIT_SUCCESS;
}