#ifndef POKERSERVER_CORE_H
#define POKERSERVER_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h> // Added for threading

#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024
#define CONCURRENT_CLIENT_NUM 100 // Updated to match servertest4.c
#define MAX_CARDS_PER_CLIENT 52

char clientCardsMap[CONCURRENT_CLIENT_NUM][MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1];
int clientSockets[CONCURRENT_CLIENT_NUM]; // New addition
pthread_mutex_t mapLock; // New addition for synchronizing access to clientCardsMap
pthread_mutex_t socketLock; // New addition for synchronizing access to clientSockets

int getPortNum(int argc, char *const *argv);

int strlen_without_null(const char *str);

char** initializeCards();
void freeCards(char** cards);

void shuffleCards(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards);

int getFirstCardsIndex(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards);

time_t currentTime;

void broadcastMessage(const char *message); // New addition
void *clientRequestHandler(void *arg); // New addition
void *handleClient(void *arg); // New addition

#endif
