/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* servertest3.c: Main file for the server			                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024
#define CONCURRENT_CLIENT_NUM 100 // Increased to handle more clients
#define MAX_CARDS_PER_CLIENT 52

char clientCardsMap[CONCURRENT_CLIENT_NUM][MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1];
pthread_mutex_t mapLock; // Mutex for synchronizing access to clientCardsMap

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

    // shapes
    char* shapes[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    // marks
    char* marks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 13; ++j) {
            char* card = (char*)malloc((strlen(marks[j]) + strlen(shapes[i]) + 2) * sizeof(char));
            if (card == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(card, marks[j]);
            strcat(card, "-");
            strcat(card, shapes[i]);
            cards[index++] = card;
        }
    }
    return cards;
}

void shuffleCards(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards) {
    srand(time(NULL)); // Seed the random number generator
    char temp[MAX_MESSAGE_LEN + 1];
    for (int i = numCards - 1; i > 0; i--) {
        // Generate a random index between 0 and i (inclusive)
        int j = rand() % (i + 1);

        // Swap cards[i] and cards[j]
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

void *handleClient(void *arg) {
    int dataSocket = *((int *)arg);
    free(arg);

    char receiveBuffer[MAX_MESSAGE_LEN + 1];
    char sendBuffer[MAX_MESSAGE_LEN + 20];
    int bytesReceived, bytesSent;

    pthread_mutex_lock(&mapLock); // Lock mutex
    if (clientCardsMap[dataSocket][0][0] == '\0') {
        char **cards = initializeCards();
        for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
            strcpy(clientCardsMap[dataSocket][i], cards[i]);
            free(cards[i]);
        }
        free(cards);
        shuffleCards(clientCardsMap[dataSocket], MAX_CARDS_PER_CLIENT);
    }
    pthread_mutex_unlock(&mapLock); // Unlock mutex

    do {
        bytesReceived = read(dataSocket, receiveBuffer, sizeof(receiveBuffer) - 1);
        if (bytesReceived < 0) {
            fprintf(stderr, "reading from data socket failed\n");
            pthread_exit(NULL);
        }
        receiveBuffer[bytesReceived] = '\0';
        printf("message Received: %s\n", receiveBuffer);

        if (strncmp(receiveBuffer, "bye", strlen_without_null(receiveBuffer)) == 0) {
            sprintf(sendBuffer, "server bye");
            bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
            if (bytesSent < 0) {
                fprintf(stderr, "writing to data socket failed\n");
                pthread_exit(NULL);
            }
            printf("Message Sent: %s\n", sendBuffer);
            break;
        } else if (strncmp(receiveBuffer, "shutdown", strlen_without_null(receiveBuffer)) == 0) {
            sprintf(sendBuffer, "server shutdown");
            bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
            if (bytesSent < 0) {
                fprintf(stderr, "writing to data socket failed\n");
                pthread_exit(NULL);
            }
            printf("Message Sent: %s\n", sendBuffer);
            break;
        } else if (strncmp(receiveBuffer, "current time", strlen_without_null(receiveBuffer)) == 0) {
            time(&currentTime);
            sprintf(sendBuffer, "Current time is: %s", ctime(&currentTime));
        } else if (strncmp(receiveBuffer, "give me cards", strlen_without_null(receiveBuffer)) == 0) {
            pthread_mutex_lock(&mapLock); // Lock mutex for access
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
            pthread_mutex_unlock(&mapLock); // Unlock mutex after access
        } else {
            sprintf(sendBuffer, "server: invalid message '%s'", receiveBuffer);
        }
        bytesSent = write(dataSocket, sendBuffer, strlen_without_null(sendBuffer));
        if (bytesSent < 0) {
            fprintf(stderr, "writing to data socket failed\n");
            pthread_exit(NULL);
        }
        printf("Message Sent: %s\n", sendBuffer);
    } while (strncmp(receiveBuffer, "shutdown", strlen_without_null(receiveBuffer)) != 0);
    close(dataSocket);
    pthread_exit(NULL);
}

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

    pthread_mutex_init(&mapLock, NULL); // Initialize mutex

    while (1) {
        int *dataSocket = malloc(sizeof(int));
        if (!dataSocket) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        *dataSocket = accept(socketFD, NULL, NULL);
        if (*dataSocket < 0) {
            fprintf(stderr, "accepting client connection failed\n");
            free(dataSocket);
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handleClient, dataSocket) != 0) {
            fprintf(stderr, "thread creation failed\n");
            free(dataSocket);
            continue;
        }

        pthread_detach(tid);
    }

    pthread_mutex_destroy(&mapLock); // Destroy mutex
    close(socketFD);
    return 0;
}
