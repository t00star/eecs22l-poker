/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   	     */
/* pokersever.c: Main file for the sever	                     */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h> // Added for threading

#include "pokerserver_core.h"

/*
#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024
#define CONCURRENT_CLIENT_NUM 5
#define MAX_CARDS_PER_CLIENT 52
*/

char clientCardsMap[CONCURRENT_CLIENT_NUM][MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1];
int clientSockets[CONCURRENT_CLIENT_NUM];
pthread_mutex_t mapLock; /* Mutex for synchronizing access to clientCardsMap */
pthread_mutex_t socketLock; /* Mutex for synchronizing access to clientSockets */

int getPortNum(int argc, char *const *argv) {
    if (argc != 2) {
        fprintf(stderr, "wrong params. Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int portNum = atoi(argv[1]);
    if (portNum <= PORT_NUMBER_LOWER_BOUND) {
        fprintf(stderr, "[%s] invalid port number %d, should be greater than %d\n", argv[0], portNum, PORT_NUMBER_LOWER_BOUND);
        exit(EXIT_FAILURE);
    }
    return portNum;
}

int strlen_without_null(const char *str) {
    int length = 0;
    while (*str != '\0' && *str != '\n') {
        ++length;
        ++str;
    }
    return length;
}

char** initializeCards() {
    char** cards = (char**)malloc(MAX_CARDS_PER_CLIENT * sizeof(char*));
    if (cards == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    /* shapes */
    char* shapes[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    /* marks */
    char* marks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 13; ++j) {
            cards[index] = (char*)malloc((strlen(marks[j]) + strlen(shapes[i]) + 2) * sizeof(char));
            if (cards[index] == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(cards[index], marks[j]);
            strcat(cards[index], "-");
            strcat(cards[index], shapes[i]);
            index++;
        }
    }
    return cards;
}

void freeCards(char** cards) {
    for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
        free(cards[i]);
    }
    free(cards);
}

void shuffleCards(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards) {
    srand(time(NULL)); // Seed the random number generator
    char temp[MAX_MESSAGE_LEN + 1];
    for (int i = numCards - 1; i > 0; i--) {
        /* Generate a random index between 0 and i (inclusive) */
        int j = rand() % (i + 1);

        /* Swap cards[i] and cards[j] */
        strcpy(temp, cards[i]);
        strcpy(cards[i], cards[j]);
        strcpy(cards[j], temp);
    }
}

int getFirstCardsIndex(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards) {
    for (int i = numCards - 1; i > 0; --i) {
        if (strcmp(cards[i], "SENT") != 0) {
            return i;
        }
    }
    return -1;
}

time_t currentTime;

void broadcastMessage(const char *message) {
    pthread_mutex_lock(&socketLock);
    for (int i = 0; i < CONCURRENT_CLIENT_NUM; ++i) {
        if (clientSockets[i] != 0) {
            write(clientSockets[i], message, strlen_without_null(message));
        }
    }
    pthread_mutex_unlock(&socketLock);
}

void *clientRequestHandler(void *arg) {
    int dataSocket = *((int *)arg);
    free(arg); /* Free the dynamically allocated socket descriptor */

    char receiveBuffer[MAX_MESSAGE_LEN + 1];
    char sendBuffer[MAX_MESSAGE_LEN + 20];
    int bytesReceived, bytesSent;

    while (1) { /* Change to an infinite loop */
        bytesReceived = read(dataSocket, receiveBuffer, sizeof(receiveBuffer) - 1);
        if (bytesReceived < 0) {
            fprintf(stderr, "reading from data socket failed\n");
            close(dataSocket);
            pthread_exit(NULL);
        }
        receiveBuffer[bytesReceived] = '\0';
        printf("Message Received: %s\n", receiveBuffer);

        if (strncmp(receiveBuffer, "bye", strlen_without_null(receiveBuffer)) == 0) {
            sprintf(sendBuffer, "server bye");
            bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
            if (bytesSent < 0) {
                fprintf(stderr, "writing to data socket failed\n");
                close(dataSocket);
                pthread_exit(NULL);
            }
            printf("Message Sent: %s\n", sendBuffer);
            break;
        } else if (strncmp(receiveBuffer, "shutdown", strlen_without_null(receiveBuffer)) == 0) {
            sprintf(sendBuffer, "server shutdown");
            bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
            if (bytesSent < 0) {
                fprintf(stderr, "writing to data socket failed\n");
                close(dataSocket);
                pthread_exit(NULL);
            }
            printf("Message Sent: %s\n", sendBuffer);
            break;
        } else if (strncmp(receiveBuffer, "current time", strlen_without_null(receiveBuffer)) == 0) {
            time(&currentTime);
            sprintf(sendBuffer, "Current time is: %s", ctime(&currentTime));
        } else if (strncmp(receiveBuffer, "give me cards", strlen_without_null(receiveBuffer)) == 0) {
            pthread_mutex_lock(&mapLock); /* Lock mutex for access */
            int cardIndex = getFirstCardsIndex(clientCardsMap[dataSocket], MAX_CARDS_PER_CLIENT);
            if (cardIndex == -1) {
                sprintf(sendBuffer, "No more cards to deal");
            } else {
                sprintf(sendBuffer, "Card 1: %s, Card 2: %s",
                        clientCardsMap[dataSocket][cardIndex],
                        clientCardsMap[dataSocket][cardIndex - 1]);
                strcpy(clientCardsMap[dataSocket][cardIndex], "SENT");
                strcpy(clientCardsMap[dataSocket][cardIndex - 1], "SENT");
            }
            pthread_mutex_unlock(&mapLock); /* Unlock mutex after access */
        } else {
            sprintf(sendBuffer, "server: invalid message '%s'", receiveBuffer);
        }
        bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
        if (bytesSent < 0) {
            fprintf(stderr, "writing to data socket failed\n");
            close(dataSocket);
            pthread_exit(NULL);
        }
        printf("Message Sent: %s\n", sendBuffer);
    }

    close(dataSocket);

    /* Remove client socket from the list */
    pthread_mutex_lock(&socketLock);
    for (int i = 0; i < CONCURRENT_CLIENT_NUM; ++i) {
        if (clientSockets[i] == dataSocket) {
            clientSockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&socketLock);

    pthread_exit(NULL);
}

void *handleClient(void *arg) {
    int dataSocket = *((int *)arg);
    free(arg); /* Free the dynamically allocated socket descriptor */

    pthread_mutex_lock(&mapLock); /* Lock mutex */
    if (clientCardsMap[dataSocket][0][0] == '\0') {
        char **cards = initializeCards();
        for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
            strcpy(clientCardsMap[dataSocket][i], cards[i]);
            free(cards[i]);
        }
        free(cards);
        shuffleCards(clientCardsMap[dataSocket], MAX_CARDS_PER_CLIENT);
    }
    pthread_mutex_unlock(&mapLock); /* Unlock mutex */

    /* Add client socket to the list */
    pthread_mutex_lock(&socketLock);
    for (int i = 0; i < CONCURRENT_CLIENT_NUM; ++i) {
        if (clientSockets[i] == 0) {
            clientSockets[i] = dataSocket;
            break;
        }
    }
    pthread_mutex_unlock(&socketLock);

    pthread_t requestThread;
    int *dataSocketPtr = malloc(sizeof(int));
    if (dataSocketPtr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        close(dataSocket);
        pthread_exit(NULL);
    }
    *dataSocketPtr = dataSocket;

    if (pthread_create(&requestThread, NULL, clientRequestHandler, dataSocketPtr) != 0) {
        fprintf(stderr, "request thread creation failed\n");
        close(dataSocket);
        free(dataSocketPtr);
        pthread_exit(NULL);
    }
    pthread_detach(requestThread);
    pthread_exit(NULL);
}

