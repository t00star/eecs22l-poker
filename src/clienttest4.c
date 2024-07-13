#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024

void getPortNum(int argc, char *const *argv, int* portNum) {
    if (argc < 3) {
        fprintf(stderr, "wrong params. Usage: %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    *portNum = atoi(argv[2]);
    if (*portNum <= PORT_NUMBER_LOWER_BOUND) {
        fprintf(stderr, "[%s] invalid port number %d, should be greater than %d\n", argv[0], *portNum, PORT_NUMBER_LOWER_BOUND);
        exit(EXIT_FAILURE);
    }
}

void *listenForMessages(void *arg) {
    int socketFD = *((int *)arg);
    char recvBuffer[MAX_MESSAGE_LEN + 1];
    int BytesReceived;

    while (1) {
        BytesReceived = read(socketFD, recvBuffer, sizeof(recvBuffer) - 1);
        if (BytesReceived > 0) {
            recvBuffer[BytesReceived] = '\0';
            printf("Received message: %s\n", recvBuffer);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char *hostName = argv[1];
    int portNum = 0;
    getPortNum(argc, argv, &portNum);

    struct hostent *host = gethostbyname(hostName);
    if (host == NULL) {
        fprintf(stderr, "[%s] host name '%s' is not found\n", argv[0], hostName);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    memcpy(&serverAddr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        fprintf(stderr, "[%s] failed at socket creation\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "[%s] connecting to host failed\n", argv[0]);
        close(socketFD); // Close the socket before exiting
        exit(EXIT_FAILURE);
    }

    pthread_t listenerThread;
    int *socketPtr = malloc(sizeof(int));
    if (socketPtr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        close(socketFD);
        exit(EXIT_FAILURE);
    }
    *socketPtr = socketFD;
    if (pthread_create(&listenerThread, NULL, listenForMessages, socketPtr) != 0) {
        fprintf(stderr, "Listener thread creation failed.\n");
        close(socketFD);
        free(socketPtr);
        exit(EXIT_FAILURE);
    }

    char sendBuffer[MAX_MESSAGE_LEN + 1];

    do {
        printf("[%s] enter a message to send to the host,\n", argv[0]);
        printf("         or 'bye' to quit this client,\n");
        printf("         or 'shutdown' to quit both host and client,\n");
        printf("         or 'current time' to get the current server time,\n");
        printf("         or 'give me cards' to get 2 random cards.\n");
        printf("message: ");
        fgets(sendBuffer, sizeof(sendBuffer), stdin);
        sendBuffer[strlen(sendBuffer) - 1] = '\0';
        int Length = strlen(sendBuffer);

        if (Length) {
            printf("[%s] Sending message '%s'...\n", argv[0], sendBuffer);
            int BytesSent = write(socketFD, sendBuffer, Length);
            if (BytesSent < 0) {
                fprintf(stderr, "[%s] writing to socket failed\n", argv[0]);
                close(socketFD); // Close the socket before exiting
                exit(EXIT_FAILURE);
            }
        }
    } while (strcmp("bye", sendBuffer) != 0 && strcmp("shutdown", sendBuffer) != 0);

    printf("[%s] client is going to exit\n", argv[0]);
    close(socketFD);

    pthread_cancel(listenerThread); // Terminate the listener thread
    free(socketPtr);

    return 0;
}
