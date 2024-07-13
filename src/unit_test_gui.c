/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* unit_test_gui.c: Unit test for GUI	                             */
/*********************************************************************/
#include "gui.h"

int main (int argc, char *argv[])
{

	// String used to print the dealer cards on the poker table in the GUI
	// Notation: RANK_of_SUIT; ex: ace_of_spades

	// START of client copy
	// IMPORTANT NOTE FOR SERVER CLIENT
	// UPDATE THE INPUT STRING (IT IS CURRENTLY HARDCODED SO COMMENT THAT OUT)

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
		return 1;
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

	// END of client copy
	printf("Output: %s\n",output_string);

	// printf("menu_state: %d\n", menu_state);
	// printf("Client Username: %s\n", output_username);
	// printf("Client Password: %s\n", output_password);
	// printf("Client Seat: %d\n", output_seat_number);

	// No Errors
	return 0 ;
}

/*
Links:
https://zetcode.com/gui/gtk2/
https://docs.huihoo.com/gtk/tutorial/2.0/index.html
https://www.manpagez.com/html/gtk2/gtk2-2.24.29/
https://docs.gtk.org/gtk3/index.html
https://developer.gnome.org/documentation/tools/valgrind.html
valgrind --leak-check=full --suppressions=/usr/share/glib-2.0/valgrind/glib.supp ./GTK_ClockClient bondi 10000
*/
/*
TODO LIST: (player)
import a poker table for the background
add player names, points, and action 


Create a structure to remove the use of gobal variables
Next expand the structure to accept the combo box, int to store the combo box.
Expand it so that I'm able to switch between two windows at the click of the button.

Set the title to the title of the program by passing the user input which would be argv[0]

Control Flow for Client Side GUI:
	Start with the creation of the main menu it will return the pointer to the created window.
	Like in the ClockServer it will check the inputs from the server.
	Therefore the GUI will only update when it's in the game state and won't update if it timesout.
*/

// // Testing
// char *testing = "Card 1: king_of_diamonds, Card 2: queen_of_diamonds";
// char card_1[100] = "";
// char card_2[100] = "";
// char card_3[100] = "";
// char card_4[100] = "";
// char card_5[100] = "";
// // Copying strings
// printf("inital string: %s\n", testing);
// // Use sscanf to extract substrings
// if (sscanf(testing, "Card 1: %100[^,], Card 2: %100[^,]", card_1, card_2) == 2)
// {
//     printf("Card 1: %s\n", card_1);
//     printf("Card 2: %s\n", card_2);
// 	strncpy(INPUT_dealt_cards[0], card_1, 100);
// 	strncpy(INPUT_dealt_cards[1], card_2, 100);
// 	strncpy(INPUT_dealt_cards[2], card_3, 100);
// 	strncpy(INPUT_dealt_cards[3], card_4, 100);
// 	strncpy(INPUT_dealt_cards[4], card_5, 100);
// }

/* 
char input_string[STRING_BUFFER] = "Dealer Cards: king_of_diamonds, ace_of_spades, back_of_card, back_of_card, back_of_card; Player#1: Alex, 800, 200, 1, 1, 0, 1, king_of_diamonds, queen_of_diamonds; Player#2: Bob, 1000, 0, 0, 0, 0, 0, 10_of_diamonds, 9_of_diamonds; Player#3: Max, 1000, 0, 0, 0, 0, 0, 8_of_diamonds, 6_of_diamonds; Player#4: Cry, 900, 100, 1, 2, 1, 0, 5_of_diamonds, 4_of_diamonds; Side Pot: 300; Dealer: Here comes the turn!;";
	printf("%s\n", input_string);
	sscanf(input_string, "Dealer Cards: %1000[^,], %1000[^,], %1000[^,], %1000[^,], %1000[^;]; Player#1: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#2: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#3: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Player#4: %1000[^,], %d, %d, %d, %d, %d, %d, %1000[^,], %1000[^;]; Side Pot: %d; Dealer: %1000[^;];", INPUT_dealt_cards[0], INPUT_dealt_cards[1], INPUT_dealt_cards[2], INPUT_dealt_cards[3], INPUT_dealt_cards[4], INPUT_player_usernames[0], &INPUT_player_points[0], &INPUT_player_current_bet[0], &INPUT_player_in_game[0], &INPUT_player_action[0], &INPUT_player_turn[0], &INPUT_dealer_chip[0], INPUT_player_1_cards[0], INPUT_player_1_cards[1], INPUT_player_usernames[1], &INPUT_player_points[1], &INPUT_player_current_bet[1], &INPUT_player_in_game[1], &INPUT_player_action[1], &INPUT_player_turn[1], &INPUT_dealer_chip[1], INPUT_player_2_cards[0], INPUT_player_2_cards[1], INPUT_player_usernames[2], &INPUT_player_points[2], &INPUT_player_current_bet[2], &INPUT_player_in_game[2], &INPUT_player_action[2], &INPUT_player_turn[2], &INPUT_dealer_chip[2], INPUT_player_3_cards[0], INPUT_player_3_cards[1], INPUT_player_usernames[3], &INPUT_player_points[3], &INPUT_player_current_bet[3], &INPUT_player_in_game[3], &INPUT_player_action[3], &INPUT_player_turn[3], &INPUT_dealer_chip[3], INPUT_player_4_cards[0], INPUT_player_4_cards[1], &INPUT_side_pot, INPUT_dealer_message);

	printf("Dealer Cards: %s, %s, %s, %s, %s; Player#1: %s, %d, %d, %d, %d, %d, %d, %s, %s; Player#2: %s, %d, %d, %d, %d, %d, %d, %s, %s; Player#3: %s, %d, %d, %d, %d, %d, %d, %s, %s; Player#4: %s, %d, %d, %d, %d, %d, %d, %s, %s; Side Pot: %d; Dealer: %s;\n", INPUT_dealt_cards[0], INPUT_dealt_cards[1], INPUT_dealt_cards[2], INPUT_dealt_cards[3], INPUT_dealt_cards[4], INPUT_player_usernames[0], INPUT_player_points[0], INPUT_player_current_bet[0], INPUT_player_in_game[0], INPUT_player_action[0], INPUT_player_turn[0], INPUT_dealer_chip[0], INPUT_player_1_cards[0], INPUT_player_1_cards[1], INPUT_player_usernames[1], INPUT_player_points[1], INPUT_player_current_bet[1], INPUT_player_in_game[1], INPUT_player_action[1], INPUT_player_turn[1], INPUT_dealer_chip[1], INPUT_player_2_cards[0], INPUT_player_2_cards[1], INPUT_player_usernames[2], INPUT_player_points[2], INPUT_player_current_bet[2], INPUT_player_in_game[2], INPUT_player_action[2], INPUT_player_turn[2], INPUT_dealer_chip[2], INPUT_player_3_cards[0], INPUT_player_3_cards[1], INPUT_player_usernames[3], INPUT_player_points[3], INPUT_player_current_bet[3], INPUT_player_in_game[3], INPUT_player_action[3], INPUT_player_turn[3], INPUT_dealer_chip[3], INPUT_player_4_cards[0], INPUT_player_4_cards[1], INPUT_side_pot, INPUT_dealer_message);
 */










