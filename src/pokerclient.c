/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* pokerclient.c: Main file for the client			                 */
/*********************************************************************/

#include "pokerclient_core.h"

int main(int argc, char *argv[]) {

    // Ensure proper command line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char INPUT_dealt_cards[5][STRING_BUFFER];
	char INPUT_player_usernames[4][STRING_BUFFER];
	int INPUT_player_points[4];
	int INPUT_player_current_bet[4];
	int INPUT_player_in_game[4];	//bool
	int INPUT_player_action[4];
	int INPUT_player_turn[4];	//bool
	int INPUT_dealer_chip[4];	//bool
	char INPUT_player_1_cards[2][STRING_BUFFER];
	char INPUT_player_2_cards[2][STRING_BUFFER];
	char INPUT_player_3_cards[2][STRING_BUFFER];
	char INPUT_player_4_cards[2][STRING_BUFFER];
	int INPUT_side_pot;
	char INPUT_dealer_message[STRING_BUFFER];

	char output_string[STRING_BUFFER];
	// SERVER UPDATES THIS STRING
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	char input_string[STRING_BUFFER] = "Dealer Cards: king_of_diamonds, ace_of_spades, back_of_card, back_of_card, back_of_card; Player#1: Alex, 800, 200, 1, 1, 0, 1, king_of_diamonds, queen_of_diamonds; Player#2: Bob, 1000, 0, 0, 0, 0, 0, 10_of_diamonds, 9_of_diamonds; Player#3: Max, 1000, 0, 0, 0, 0, 0, 8_of_diamonds, 6_of_diamonds; Player#4: Cry, 900, 100, 1, 2, 1, 0, 5_of_diamonds, 4_of_diamonds; Side Pot: 300; Dealer: Here comes the turn!;";

	// GTK Variable
	GtkWidget *window = NULL;
	// Creates a GUI for client
	window = CreateClientWindow(&argc, &argv, INPUT_dealt_cards);
	// Checks that a window pointer has been returned
	if (!window)
    {
		// Prints error to standard error stream
		fprintf(stderr, "GTKWidget pointer to window has not been returned.\n");
        // Error Code #1
		return EXIT_FAILURE;
    }
	while(!Client_Window_CLOSED)
	{
		if (gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
		{
			// UPDATES THE OUTPUT STRING
			snprintf(output_string, sizeof(output_string), "Username: %s; Password: %s; Seat: %d; Fold: %d; Call: %d; Raise: %d; All In; %d", output_username, output_password, output_seat_number, output_fold, output_call, output_raise, output_all_in);
			// SEND STRING TO SERVER HERE
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

			// reset button clicks
			output_fold = 0;
			output_call = 0;
			output_raise = 0;
			output_all_in = 0;

			// UPDATES THE GUI BASED ON INPUT STRING
			// Check if string has updated
			sscanf(input_string, "Dealer Cards: %1000[^,], %1000[^,], %1000[^,], %1000[^,], %1000[^;]; Player#1: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#2: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#3: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#4: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Side Pot: %d; Dealer: %1000[^;];", INPUT_dealt_cards[0], INPUT_dealt_cards[1], INPUT_dealt_cards[2], INPUT_dealt_cards[3], INPUT_dealt_cards[4], INPUT_player_usernames[0], &INPUT_player_points[0], &INPUT_player_current_bet[0], &INPUT_player_in_game[0], &INPUT_player_action[0], &INPUT_player_turn[0], &INPUT_dealer_chip[0], INPUT_player_1_cards[0], INPUT_player_1_cards[1], INPUT_player_usernames[1], &INPUT_player_points[1], &INPUT_player_current_bet[1], &INPUT_player_in_game[1], &INPUT_player_action[1], &INPUT_player_turn[1], &INPUT_dealer_chip[1], INPUT_player_2_cards[0], INPUT_player_2_cards[1], INPUT_player_usernames[2], &INPUT_player_points[2], &INPUT_player_current_bet[2], &INPUT_player_in_game[2], &INPUT_player_action[2], &INPUT_player_turn[2], &INPUT_dealer_chip[2], INPUT_player_3_cards[0], INPUT_player_3_cards[1], INPUT_player_usernames[3], &INPUT_player_points[3], &INPUT_player_current_bet[3], &INPUT_player_in_game[3], &INPUT_player_action[3], &INPUT_player_turn[3], &INPUT_dealer_chip[3], INPUT_player_4_cards[0], INPUT_player_4_cards[1], &INPUT_side_pot, INPUT_dealer_message);
			// Update based on string
			update_username_LABELs(INPUT_player_usernames);
			update_points_LABELs(INPUT_player_points);
			update_current_bet_LABELs(INPUT_player_current_bet);
			update_action_LABELs(INPUT_player_action);
			update_player_cards(INPUT_player_1_cards,INPUT_player_2_cards,INPUT_player_3_cards,INPUT_player_4_cards);
			update_dealt_cards(INPUT_dealt_cards);
			update_dealer_chip(INPUT_dealer_chip);
			update_dealer_message_LABEL(INPUT_dealer_message);
			update_side_pot_LABEL(INPUT_side_pot);
		}
        while(gtk_events_pending())
		{
            gtk_main_iteration();
        }
    }

    char *hostName = argv[1];
    int portNum = 0;
    getPortNum(argc, argv, &portNum);

    struct hostent *host = gethostbyname(hostName);
    if (host == NULL) {
        fprintf(stderr, "[%s] host name '%s' not found\n", argv[0], hostName);
        return EXIT_FAILURE;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    memcpy(&serverAddr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        fprintf(stderr, "[%s] Failed to create socket\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "[%s] Failed to connect to the host\n", argv[0]);
        close(socketFD);
        return EXIT_FAILURE;
    }

    // Launch a thread to handle messages
    pthread_t listenerThread;
    int *socketPtr = malloc(sizeof(int));
    if (socketPtr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        close(socketFD);
        return EXIT_FAILURE;
    }
    *socketPtr = socketFD;
    if (pthread_create(&listenerThread, NULL, listenForMessages, socketPtr) != 0) {
        fprintf(stderr, "Listener thread creation failed.\n");
        close(socketFD);
        free(socketPtr);
        return EXIT_FAILURE;
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

    // Cleanup on exit
    printf("[%s] Client is going to exit\n", argv[0]);
    close(socketFD);
    pthread_cancel(listenerThread);
    free(socketPtr);

    return 0;
}
