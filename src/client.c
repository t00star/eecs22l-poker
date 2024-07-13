/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* clienttest3.c: Main file for the client			                 */
/*********************************************************************/

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include "gui.h"

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

int main(int argc, char *argv[])
{
    
	// GTK Variables
	//GtkWidget *window = NULL;	// Main window
	
	// Creates a GUI for client
	//window = CreateClientWindow(&argc, &argv);
	// Checks that a window pointer has been returned
	//if (!window)
    //{
		// Prints error to standard error stream
	//	fprintf(stderr, "GTKWidget ointer to window has not been returned.\n");
        // Error Code #1
	//	return 1;
    //}
	
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
        exit(EXIT_FAILURE);
    }

    char sendBuffer[MAX_MESSAGE_LEN + 1];
    char recvBuffer[MAX_MESSAGE_LEN + 1];

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
                exit(EXIT_FAILURE);
            }

            int BytesReceived = read(socketFD, recvBuffer, sizeof(recvBuffer) - 1);
            if (BytesReceived < 0) {
                fprintf(stderr, "[%s] reading from socket failed\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            recvBuffer[BytesReceived] = '\0';
            printf("[%s] Received response: %s\n", argv[0], recvBuffer);
        }
    } while (strcmp("server bye", recvBuffer) != 0 && strcmp("server shutdown", recvBuffer) != 0);

    printf("[%s] client is going to exit\n", argv[0]);
    close(socketFD);

    return 0;
}
