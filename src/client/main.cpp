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
    if (argc < 3)
    {
        cout << "Usage:\n\t" << argv[0] << " <server-port> <user-name>" << endl;
        exit(EXIT_FAILURE);
    }

    int serverPort = atoi(argv[1]);
    char username[USERNAME_LEN];
    strncpy(username, argv[2], USERNAME_LEN);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        close(sockfd);
        error("Failed to get socket file descriptor.");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(serverAddr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        close(sockfd);
        error("Failed to connect to remote server.");
    }

    char sendBuffer[BUF_SIZE];
    bzero(sendBuffer, BUF_SIZE);

    strcpy(sendBuffer, username);
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) == -1)
    {
        close(sockfd);
        error("Failed to send message.");
    }

    char receiveBuffer[BUF_SIZE];
    bzero(receiveBuffer, BUF_SIZE);
    int nbytes;

    if ((nbytes = recv(sockfd, receiveBuffer, BUF_SIZE - 1, 0)) < 0)
    {
        close(sockfd);
        error("Failed receiving message.");
    }

    receiveBuffer[nbytes] = '\0';
    cout << receiveBuffer << endl;

    return EXIT_SUCCESS;
}