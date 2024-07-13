#ifndef POKERCLIENT_CORE_H
#define POKERCLIENT_CORE_H

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h> // Added

#include "gui.h"

#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024

void getPortNum(int argc, char *const *argv, int* portNum);
void *listenForMessages(void *arg); // Added

#endif
