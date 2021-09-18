#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "core/constants.h"
#include "core/errors.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:\n\t" << argv[0] << " <server-port>" << endl;
        exit(EXIT_FAILURE);
    }

    int serverPort = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        close(sockfd);
        error("Failed to get socket file descriptor.");
    }

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    memset(&(serverAddr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr)) < 0) {
        close(sockfd);
        cerr << "Failed to bind on socket!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Server running on port " << serverPort << endl;

    socklen_t address_size = sizeof(clientAddr);

    char buffer[BUF_SIZE];
    bzero(buffer, BUF_SIZE);

    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &address_size) < 0)
    {
        close(sockfd);
        error("Reading data from socket.");
    }
    cout << "Received message: " << buffer << endl;

    if (sendto(sockfd, "hi", 2, 0, (struct sockaddr *)&clientAddr, address_size) < 0)
    {
        close(sockfd);
        error("Writing to socket.");
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
