/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* pokersever.c: Main file for the sever			                 */
/*********************************************************************/

#include "pokerserver_core.h"


int main(int argc, char *argv[])
{
    int portNum = getPortNum(argc, argv);

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        fprintf(stderr, "[%s] failed at socket creation\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "[%s] failed at binding\n", argv[0]);
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    if (listen(socketFD, 5) < 0) {
        fprintf(stderr, "[%s] failed at listening\n", argv[0]);
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    printf("[%s] server is running and listening on port %d\n", argv[0], portNum);

    pthread_mutex_init(&mapLock, NULL); /* Initialize mutex */
    pthread_mutex_init(&socketLock, NULL); /* Initialize socket mutex */

    memset(clientSockets, 0, sizeof(clientSockets)); /* Initialize client sockets array */

    while (1) {
        int *dataSocket = malloc(sizeof(int));
        if (!dataSocket) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        *dataSocket = accept(socketFD, NULL, NULL);
        if (*dataSocket < 0) {
            fprintf(stderr, "accept failed\n");
            free(dataSocket);
            continue;
        }

        pthread_t clientThread;
        int *dataSocketPtr = malloc(sizeof(int));
        if (dataSocketPtr == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            close(*dataSocket);
            free(dataSocket);
            continue;
        }
        *dataSocketPtr = *dataSocket;
        free(dataSocket); /* Free the initial allocation, as it's now managed by dataSocketPtr */

        if (pthread_create(&clientThread, NULL, handleClient, dataSocketPtr) != 0) {
            fprintf(stderr, "client thread creation failed\n");
            close(*dataSocketPtr);
            free(dataSocketPtr);
            continue;
        }
        pthread_detach(clientThread);
    }

    close(socketFD);
    pthread_mutex_destroy(&mapLock); /* Destroy mutex */
    pthread_mutex_destroy(&socketLock); /* Destroy socket mutex */
    return 0;
}

