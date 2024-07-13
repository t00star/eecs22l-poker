/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                     	 */
/* GUI.h: Structs + Functionality of GUI header file				 */
/*********************************************************************/
#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <assert.h>
// #include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Contstants will be moved to constants.h later
// Defualt Window Size
// #define WINDOW_WIDTH 1280
// #define WINDOW_HEIGHT 720
// Window width & height is divided by 1.5 for ease of programming on a smaller screen
#define WINDOW_WIDTH 1280/1.2
#define WINDOW_HEIGHT 720/1.2
// Used for video
// #define WINDOW_WIDTH 1280/2.5
// #define WINDOW_HEIGHT 720/2.5
#define CARD_WIDTH (WINDOW_WIDTH/15)
#define CARD_HEIGHT (WINDOW_HEIGHT/6)
#define CHIP_WIDTH (WINDOW_WIDTH/14)
#define CHIP_HEIGHT (WINDOW_HEIGHT/8.5)
#define NUMBER_OF_SEATS 4
#define STRING_BUFFER 10000

/* Variables */
// Output Variables
extern int menu_state;
extern char output_username[STRING_BUFFER];
extern char output_password[STRING_BUFFER];
extern int output_seat_number;
extern bool output_fold;
extern bool output_call;
extern int output_raise;
extern bool output_all_in;
// GTK Update Variables
extern GtkWidget *Client_NOTEBOOK;
extern bool Client_Window_CLOSED;
extern GtkWidget *Dealer_Message_LABEL;
extern GtkWidget *side_pot_LABEL;
extern GtkWidget *CMM_ENTRY_raise;
// Dealt Cards
extern GtkWidget *CGM_dealt_card_IMAGE_1;
extern GtkWidget *CGM_dealt_card_IMAGE_2;
extern GtkWidget *CGM_dealt_card_IMAGE_3;
extern GtkWidget *CGM_dealt_card_IMAGE_4;
extern GtkWidget *CGM_dealt_card_IMAGE_5;
// Player #1
extern GtkWidget *CGM_player_1_username_LABEL;
extern GtkWidget *CGM_player_1_points_LABEL;
extern GtkWidget *CGM_player_1_current_bet_LABEL;
extern GtkWidget *CGM_player_1_action_LABEL;
extern GtkWidget *CGM_player_1_card_1;
extern GtkWidget *CGM_player_1_card_2;
extern GtkWidget *CGM_player_1_dealer_chip;
// Player #2 labels
extern GtkWidget *CGM_player_3_username_LABEL;
extern GtkWidget *CGM_player_3_points_LABEL;
extern GtkWidget *CGM_player_3_current_bet_LABEL;
extern GtkWidget *CGM_player_3_action_LABEL;
extern GtkWidget *CGM_player_3_card_1;
extern GtkWidget *CGM_player_3_card_2;
extern GtkWidget *CGM_player_3_dealer_chip;
// Player #3 labels
extern GtkWidget *CGM_player_2_username_LABEL;
extern GtkWidget *CGM_player_2_points_LABEL;
extern GtkWidget *CGM_player_2_current_bet_LABEL;
extern GtkWidget *CGM_player_2_action_LABEL;
extern GtkWidget *CGM_player_2_card_1;
extern GtkWidget *CGM_player_2_card_2;
extern GtkWidget *CGM_player_2_dealer_chip;
// Player #4 labels
extern GtkWidget *CGM_player_4_username_LABEL;
extern GtkWidget *CGM_player_4_points_LABEL;
extern GtkWidget *CGM_player_4_current_bet_LABEL;
extern GtkWidget *CGM_player_4_action_LABEL;
extern GtkWidget *CGM_player_4_card_1;
extern GtkWidget *CGM_player_4_card_2;
extern GtkWidget *CGM_player_4_dealer_chip;

/* Function Declarations */
// CreateClientWindow creates the GUI for the client side of the poker program
GtkWidget *CreateClientWindow(int *argc, char **argv[], char dealt_cards[5][STRING_BUFFER]);
// Test Function that to updates the placeholder label
void update_dealer_message_LABEL(char *input_value);
void update_username_LABELs(char INPUT_player_usernames[4][STRING_BUFFER]);
void update_points_LABELs(int INPUT_player_points[4]);
void update_current_bet_LABELs(int INPUT_player_current_bet[4]);
void update_action_LABELs(int INPUT_player_action[4]);
void update_player_cards(char INPUT_player_1_cards[2][STRING_BUFFER], char INPUT_player_2_cards[2][STRING_BUFFER], char INPUT_player_3_cards[2][STRING_BUFFER], char INPUT_player_4_cards[2][STRING_BUFFER]);
void update_dealt_cards(char INPUT_dealt_cards[5][STRING_BUFFER]);
void update_dealer_chip(int INPUT_dealer_chip[4]);
void update_side_pot_LABEL(int input_value);
void update_dealer_chip(int INPUT_dealer_chip[4]);
void update_poker_card(GtkWidget* image_to_be_updated, char *input_poker_card);
void update_player_cards(char INPUT_player_1_cards[2][STRING_BUFFER], char INPUT_player_2_cards[2][STRING_BUFFER], char INPUT_player_3_cards[2][STRING_BUFFER], char INPUT_player_4_cards[2][STRING_BUFFER]);
void update_dealt_cards(char INPUT_dealt_cards[5][STRING_BUFFER]);
void update_action_LABELs(int INPUT_player_action[4]);
void update_current_bet_LABELs(int INPUT_player_current_bet[4]);
void update_points_LABELs(int INPUT_player_points[4]);
void update_username_LABELs(char INPUT_player_usernames[4][STRING_BUFFER]);
GtkWidget *create_dealer_chip(void);
GtkWidget *create_placeholder_label(char *placeholder_text, GdkColor font_COLOR);

/* Callback Function Declarations */
// delete_event is connected whenever the x on the GUI window is clicked.
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
// destroy is connected whenever a destory event occurs (whenever delete_event returns FALSE)
void destroy(GtkWidget *widget, gpointer data);
// Updates the output_username
void update_output_username(GtkEntry *CMM_ENTRY_username, char *output_username);
// Updates the output_password
void update_output_password(GtkEntry *CMM_ENTRY_password, char *output_password);
// update_output_seat_number is connected whenever the user selects a new seat from the drop down menu and it saves the seat for the current user into an integer
void update_output_seat_number(GtkComboBox *CMM_COMBO_seat_dropdown, int *client_seat);
// CMM_BUTTON_play_clicked is connected whenever the play button on the main menu is clicked. It saves the username and password the user has entered and switches to the game window
void CMM_BUTTON_play_clicked(GtkWidget *widget, gpointer data);
void CGM_BUTTON_fold_clicked(GtkWidget *widget, gpointer data);
void CGM_BUTTON_call_clicked(GtkWidget *widget, gpointer data);
void CGM_BUTTON_raise_clicked(GtkWidget *widget, gpointer data);
void CGM_BUTTON_all_in_clicked(GtkWidget *widget, gpointer data);

/* Helper Function Declarations */
// Updates the settings for the window
void update_window_settings(GtkWidget *Client_WINDOW);
// set_event_box_background_image sets the background for an event box (for each page in the Notebook)
void set_event_box_background_image(GtkWidget* event_box, char *image_file_path);
// Matches a card Suit and Rank with the image's file path
void get_card_file_path(char *card_in_string_format, int size_of_card_in_string_format);
// Create a poker card with a given file path
GtkWidget *create_poker_card(char *poker_card_file_path);
void update_poker_card(GtkWidget* image_to_be_updated, char *input_poker_card);


#endif