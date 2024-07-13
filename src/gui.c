/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* GUI.c: source file for GUI creation	                             */
/*********************************************************************/
#include "gui.h"

// Output Variables
int menu_state = 0;
char output_username[STRING_BUFFER] = "";
char output_password[STRING_BUFFER] = "";
int output_seat_number = 1;
bool output_fold;
bool output_call;
int output_raise;
bool output_all_in;
// GTK Update Variables
GtkWidget *Client_NOTEBOOK = NULL;		// Top level notebook to switch between the main menu and the game menu
bool Client_Window_CLOSED = FALSE;
// Testings Window Switching with a button click
GtkWidget *Dealer_Message_LABEL = NULL;		// Game Menu Label
GtkWidget *side_pot_LABEL = NULL;
// Dealt Cards
GtkWidget *CMM_ENTRY_raise = NULL;
GtkWidget *CGM_dealt_card_IMAGE_1 = NULL;		// First dealt card on table
GtkWidget *CGM_dealt_card_IMAGE_2 = NULL;		// Second dealt card on table
GtkWidget *CGM_dealt_card_IMAGE_3 = NULL;		// Third dealt card on table
GtkWidget *CGM_dealt_card_IMAGE_4 = NULL;		// Fourth dealt card on table
GtkWidget *CGM_dealt_card_IMAGE_5 = NULL;		// Fifth dealt card on table
// Player #1
GtkWidget *CGM_player_1_username_LABEL = NULL;
GtkWidget *CGM_player_1_points_LABEL = NULL;
GtkWidget *CGM_player_1_current_bet_LABEL = NULL;
GtkWidget *CGM_player_1_action_LABEL = NULL;
GtkWidget *CGM_player_1_card_1 = NULL;
GtkWidget *CGM_player_1_card_2 = NULL;
GtkWidget *CGM_player_1_dealer_chip = NULL;
// Player #3 labels
GtkWidget *CGM_player_3_username_LABEL = NULL;
GtkWidget *CGM_player_3_points_LABEL = NULL;
GtkWidget *CGM_player_3_current_bet_LABEL = NULL;
GtkWidget *CGM_player_3_action_LABEL = NULL;
GtkWidget *CGM_player_3_card_1 = NULL;
GtkWidget *CGM_player_3_card_2 = NULL;
GtkWidget *CGM_player_3_dealer_chip = NULL;
// Player #2 labels
GtkWidget *CGM_player_2_username_LABEL = NULL;
GtkWidget *CGM_player_2_points_LABEL = NULL;
GtkWidget *CGM_player_2_current_bet_LABEL = NULL;
GtkWidget *CGM_player_2_action_LABEL = NULL;
GtkWidget *CGM_player_2_card_1 = NULL;
GtkWidget *CGM_player_2_card_2 = NULL;
GtkWidget *CGM_player_2_dealer_chip = NULL;
// Player #4 labels
GtkWidget *CGM_player_4_username_LABEL = NULL;
GtkWidget *CGM_player_4_points_LABEL = NULL;
GtkWidget *CGM_player_4_current_bet_LABEL = NULL;
GtkWidget *CGM_player_4_action_LABEL = NULL;
GtkWidget *CGM_player_4_card_1 = NULL;
GtkWidget *CGM_player_4_card_2 = NULL;
GtkWidget *CGM_player_4_dealer_chip = NULL;

// Changes the style of an event box to a given image
void set_event_box_background_image(GtkWidget* event_box, char *image_file_path)
{
	// Variables
    GdkPixbuf* pixbuf = NULL;
	GdkPixmap *pixmap = NULL;
	GtkStyle* style_original = NULL;
	GtkStyle* style_copy = NULL;
	int window_width = NULL;
	int window_height = NULL;

	// Load the pixbuff
	pixbuf = gdk_pixbuf_new_from_file(image_file_path, NULL);
	// Error if the image can't be loaded
	if (!pixbuf)
	{
        fprintf(stderr, "Error: Unable to load image!\n\tFile Path: %s\n", image_file_path);
        return;
    }
	// Traverse upwards to find the top-level window
    GtkWidget *top_level = gtk_widget_get_toplevel(event_box);
    // Check if the top-level widget exists and is a window
	if (GTK_IS_WINDOW(top_level))
	{
        // Get the size of the window
        gtk_window_get_size(GTK_WINDOW(top_level), &window_width, &window_height);
    }
	// Set the scale of the image to the window's width/height
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, window_width, window_height, GDK_INTERP_BILINEAR);
    // Creates a GdkPixmap
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 255);
	if (!pixmap)
	{
		fprintf(stderr, "Error: Unable to create pixmap from pixbuff!\n");
        g_object_unref(pixbuf);
        return;
    }
	// Copies the style of the event box
    style_original = gtk_widget_get_style(event_box);
    style_copy = gtk_style_copy(style_original);
	// Sets the background of the copied style to the generated pixmap 
    style_copy->bg_pixmap[GTK_STATE_NORMAL] = pixmap;
	// Sets the event_box style to the style with the new background
    gtk_widget_set_style(event_box, style_copy);
	
	// Unreference GTK widgets
	g_object_unref(pixbuf);
	g_object_unref(style_copy);
}

GtkWidget *CreateClientWindow(int *argc, char **argv[], char dealt_cards[5][STRING_BUFFER])
{
	// GTK Variables
	GtkWidget *Client_WINDOW = NULL;				// Main client window
	GtkWidget *Client_notebook_PAGE_1 = NULL;		// Notebook page #1
	GtkWidget *Client_notebook_PAGE_2 = NULL;		// Notebook page #2
	GdkColor Client_white_font_COLOR;				// White font Color for labels

	// GTK Variables for the Client Main Menu (CMM)
	GtkWidget *CMM_vertical_box_ALIGNMENT = NULL;	// Alignment for the user input and play button
	GtkWidget *CMM_top_V_BOX = NULL;				// Top level VBox Widget
	// GtkWidget *CMM_menu_LABEL = NULL;				// Main Menu Label
	GtkWidget *CMM_input_table_ALIGNMENT = NULL;	// Alignment for Username/Password table
	GtkWidget *CMM_input_TABLE_menu = NULL;			// Table widget for Username/Password
	GtkWidget *CMM_LABEL_username = NULL;			// Username Label
	GtkWidget *CMM_LABEL_password = NULL;			// Password Label
	GtkWidget *CMM_ENTRY_username = NULL;			// Entry textbox for username
	GtkWidget *CMM_ENTRY_password = NULL;			// Entry textbox for password
	GtkWidget *CMM_combo_ALIGNMENT = NULL;			// Alignment for dropdown selection
	GtkWidget *CMM_COMBO_seat_dropdown = NULL;		// Dropdown text selection for seat number
	GtkWidget *CMM_play_button_ALIGNMENT = NULL;	// Alignment for the play button
	GtkWidget *CMM_BUTTON_play = NULL;				// Play button

	// GTK Variables for the Client GAME Menu (CGM)
	GtkWidget *CGM_top_H_BOX = NULL;
	GtkWidget *CGM_left_V_BOX = NULL;
	GtkWidget *CGM_middle_V_BOX = NULL;		// Allignment
	GtkWidget *CGM_right_V_BOX = NULL;
	// GtkWidget *Dealer_Message_LABEL = NULL;				// Game Menu Label
	
	// GtkWidget *IMAGE_poker_card = NULL;				// Used to load the images for the playing cards
	GtkWidget *CGM_dealt_cards_ALIGNMENT = NULL;	// Alignment for the dealth cards
	GtkWidget *CGM_dealt_cards_H_BOX = NULL;		// HBox to hold the dealth cards

	GtkWidget *CGM_BUTTON_H_BOX = NULL;				// Button box to hold all the buttons on the bottom
	GtkWidget *CGM_BUTTON_fold = NULL;				// Fold button
	GtkWidget *CGM_BUTTON_call = NULL;				// Call button
	GtkWidget *CGM_BUTTON_raise = NULL;				// Raise button
	GtkWidget *CGM_BUTTON_all_in = NULL;			// All in button

	// Non-GTK variables
	// int menu_state = 0;
	char seat[] = "Seat _";


	// char username[MAX_STRING_BUFFER];
	printf("menu_state: %d\n",menu_state);
	// Boolean used to determine if GTK has been initalized correctly
	bool initGTK = 0;

	/* GTK & Window Initalization */
	// Initalizes GTK
	initGTK = gtk_init_check(argc,argv);
	if (initGTK != 1)
	{
		// Prints error to standard error stream
		fprintf(stderr, "Error Code: %d\nCan not open external display!\n", initGTK);
		// Error Code #1 - Display can not be opened
		exit(1);
	}

	// Create a new window
	Client_WINDOW = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// Updates the settings for the window
	update_window_settings(Client_WINDOW);
	
	// Create the client notebook to switch between the different windows
	Client_NOTEBOOK = gtk_notebook_new();
	// Turn off tabs
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(Client_NOTEBOOK), FALSE);
	gtk_container_add(GTK_CONTAINER(Client_WINDOW), Client_NOTEBOOK);

	/* Main Menu Creation */
	// Create the first notebook page
	Client_notebook_PAGE_1 = gtk_event_box_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(Client_NOTEBOOK), Client_notebook_PAGE_1, NULL);
	// Set the background image of the first notebook page
	set_event_box_background_image(Client_notebook_PAGE_1, "gui_images/CMM_BG.jpeg");

	// Creates a vertical box for the main menu alignment
	CMM_vertical_box_ALIGNMENT = gtk_alignment_new(0.5, 0.6, 1, 0.65);
	gtk_container_add(GTK_CONTAINER(Client_notebook_PAGE_1), CMM_vertical_box_ALIGNMENT);
	CMM_top_V_BOX = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(CMM_vertical_box_ALIGNMENT), CMM_top_V_BOX);

	// Creates the username/password labels and entry boxes
	CMM_input_table_ALIGNMENT = gtk_alignment_new(0.475, 1, 0, 0);
	gtk_box_pack_start(GTK_BOX(CMM_top_V_BOX), CMM_input_table_ALIGNMENT, TRUE, TRUE, 0);
	// Create a table for aligning the Username/Password section
	CMM_input_TABLE_menu = gtk_table_new(2, 2, TRUE);
	// Create labels for username and password input
	CMM_LABEL_username = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(CMM_LABEL_username), "<span size='xx-large'>Username:</span>");
	CMM_LABEL_password = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(CMM_LABEL_password), "<span size='xx-large'>Password:</span>");
	// Change Label Color
    gdk_color_parse("#ffffff", &Client_white_font_COLOR);
    gtk_widget_modify_fg(CMM_LABEL_username, GTK_STATE_NORMAL, &Client_white_font_COLOR);
	gtk_widget_modify_fg(CMM_LABEL_password, GTK_STATE_NORMAL, &Client_white_font_COLOR);
	// Create entries for user to input a username and password
	CMM_ENTRY_username = gtk_entry_new();
	CMM_ENTRY_password = gtk_entry_new();
	// Hide password input
	gtk_entry_set_visibility(GTK_ENTRY(CMM_ENTRY_password), FALSE);
	// Attach labels and entries to the table
	gtk_table_attach(GTK_TABLE(CMM_input_TABLE_menu), CMM_LABEL_username, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	gtk_table_attach(GTK_TABLE(CMM_input_TABLE_menu), CMM_LABEL_password, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	gtk_table_attach(GTK_TABLE(CMM_input_TABLE_menu), CMM_ENTRY_username, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 0);
	gtk_table_attach(GTK_TABLE(CMM_input_TABLE_menu), CMM_ENTRY_password, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 0);
	// Adds the Username/Password table to the table alignment
	gtk_container_add(GTK_CONTAINER(CMM_input_table_ALIGNMENT), CMM_input_TABLE_menu);

	// Creates the seat selection dropdown menu
	CMM_combo_ALIGNMENT = gtk_alignment_new(0.5, 0.1, 0.246, 0.3);
	gtk_box_pack_start(GTK_BOX(CMM_top_V_BOX), CMM_combo_ALIGNMENT, TRUE, TRUE, 0);
	// Create a combo box text so that users can select a seat
	CMM_COMBO_seat_dropdown = gtk_combo_box_new_text();
	// Populate options for the seat dropdown selection 
	for (int i = 0; i < NUMBER_OF_SEATS; i++)
	{
		seat[5] = '1' + i; 
		// printf("%s\n", seat);
		gtk_combo_box_append_text(GTK_COMBO_BOX(CMM_COMBO_seat_dropdown), seat);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(CMM_COMBO_seat_dropdown), 0);
	// Adds the dropdown menu to the combo alignment
	gtk_container_add(GTK_CONTAINER(CMM_combo_ALIGNMENT), CMM_COMBO_seat_dropdown);

	// Creates the play button
	CMM_play_button_ALIGNMENT = gtk_alignment_new(0.5, 0.5, 0.2, 0.1);
	gtk_box_pack_start(GTK_BOX(CMM_top_V_BOX), CMM_play_button_ALIGNMENT, TRUE, TRUE, 0);
	// Creates a button to play the game
	CMM_BUTTON_play = gtk_button_new_with_label("Play");
	gtk_widget_set_tooltip_text(CMM_BUTTON_play, "Click to start game");
	gtk_container_add(GTK_CONTAINER(CMM_play_button_ALIGNMENT), CMM_BUTTON_play);


	/* Game Menu Creation */
	// Create the second notebook page
	Client_notebook_PAGE_2 = gtk_event_box_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(Client_NOTEBOOK), Client_notebook_PAGE_2, NULL);
	// Set the background image of the second notebook page
	set_event_box_background_image(Client_notebook_PAGE_2, "gui_images/CGM_BG.jpeg");

	// Create the top level horizontal box  for the game menu alignment
	CGM_top_H_BOX = gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(Client_notebook_PAGE_2), CGM_top_H_BOX);

	// Create a left vertical box for 1st and 3rd player
	CGM_left_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_top_H_BOX), CGM_left_V_BOX);
	// Create a middle vertical box for the dealer cards and player options
	CGM_middle_V_BOX = gtk_vbox_new(TRUE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_top_H_BOX), CGM_middle_V_BOX);
	// Create a right vertical box for 2nd and 4th player
	CGM_right_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_top_H_BOX), CGM_right_V_BOX);


	GtkWidget *Dealer_V_BOX = NULL;
	Dealer_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(CGM_middle_V_BOX), Dealer_V_BOX, TRUE, TRUE, 0);

	// Create an updatable label that's used for dealer messages
	Dealer_Message_LABEL = create_placeholder_label("Placeholder", Client_white_font_COLOR);
	gtk_box_pack_start(GTK_BOX(Dealer_V_BOX), Dealer_Message_LABEL, FALSE, FALSE, 0);

	// Create an updatable label for the side pot
	side_pot_LABEL = create_placeholder_label("Placeholder", Client_white_font_COLOR);
	gtk_box_pack_start(GTK_BOX(Dealer_V_BOX), side_pot_LABEL, FALSE, FALSE, 0);

	// Testing add 5 cards to the poker table (then try updating the cards using an array)
	CGM_dealt_cards_ALIGNMENT = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_middle_V_BOX), CGM_dealt_cards_ALIGNMENT, TRUE, TRUE, 0);
	CGM_dealt_cards_H_BOX = gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_dealt_cards_ALIGNMENT), CGM_dealt_cards_H_BOX);
	// Creation of the card images
	CGM_dealt_card_IMAGE_1 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_dealt_cards_H_BOX), CGM_dealt_card_IMAGE_1, TRUE, TRUE, 0);
	CGM_dealt_card_IMAGE_2 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_dealt_cards_H_BOX), CGM_dealt_card_IMAGE_2, TRUE, TRUE, 0);
	CGM_dealt_card_IMAGE_3 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_dealt_cards_H_BOX), CGM_dealt_card_IMAGE_3, TRUE, TRUE, 0);
	CGM_dealt_card_IMAGE_4 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_dealt_cards_H_BOX), CGM_dealt_card_IMAGE_4, TRUE, TRUE, 0);
	CGM_dealt_card_IMAGE_5 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_dealt_cards_H_BOX), CGM_dealt_card_IMAGE_5, TRUE, TRUE, 0);

	GtkWidget *CGM_BUTTON_H_BOX_ALLIGNMENT = NULL;
	CGM_BUTTON_H_BOX_ALLIGNMENT = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_middle_V_BOX), CGM_BUTTON_H_BOX_ALLIGNMENT, TRUE, TRUE, 0);
	// Create the buttons for player input
	CGM_BUTTON_H_BOX = gtk_hbox_new(FALSE, 10);
	// gtk_button_box_set_layout(GTK_BUTTON_BOX(CGM_BUTTON_H_BOX), GTK_BUTTONBOX_CENTER);
	gtk_container_add(GTK_CONTAINER(CGM_BUTTON_H_BOX_ALLIGNMENT), CGM_BUTTON_H_BOX);
	// Creates a button to FOLD in the game
	CGM_BUTTON_fold = gtk_button_new_with_label("Fold");
	gtk_widget_set_tooltip_text(CGM_BUTTON_fold, "Click to fold");
	// Creates a button to CALL in the game
	CGM_BUTTON_call = gtk_button_new_with_label("Call");
	gtk_widget_set_tooltip_text(CGM_BUTTON_call, "Click to call");
	// Creates a button to RAISE in the game
	CMM_ENTRY_raise = gtk_entry_new();
	CGM_BUTTON_raise = gtk_button_new_with_label("Raise");
	gtk_widget_set_tooltip_text(CGM_BUTTON_raise, "Click to raise");
	// Creates a button to go ALL IN in the game
	CGM_BUTTON_all_in = gtk_button_new_with_label("All in");
	gtk_widget_set_tooltip_text(CGM_BUTTON_all_in, "Click to go all in");
	// Packs all the buttons into the button box
	gtk_box_pack_start(GTK_BOX(CGM_BUTTON_H_BOX), CGM_BUTTON_fold, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_BUTTON_H_BOX), CGM_BUTTON_call, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_BUTTON_H_BOX), CMM_ENTRY_raise, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_BUTTON_H_BOX), CGM_BUTTON_raise, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_BUTTON_H_BOX), CGM_BUTTON_all_in, TRUE, TRUE, 0);

	// gtk_widget_set_sensitive(CGM_BUTTON_raise, FALSE);

	GtkWidget *CGM_player_1_ALIGNMENT = NULL;
	GtkWidget *CGM_player_1_V_BOX = NULL;
	GtkWidget *CGM_player_1_H_BOX = NULL;
	// Create the 1st player
	// Alignment
	CGM_player_1_ALIGNMENT = gtk_alignment_new(0.1, 0.1, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_left_V_BOX), CGM_player_1_ALIGNMENT, TRUE, TRUE, 0);
	CGM_player_1_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_player_1_ALIGNMENT), CGM_player_1_V_BOX);
	// Label Creation
	CGM_player_1_username_LABEL = create_placeholder_label("Player 1: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_1_points_LABEL = create_placeholder_label("Points: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_1_current_bet_LABEL = create_placeholder_label("Current Bet: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_1_action_LABEL = create_placeholder_label("Action: PLACEHOLDER", Client_white_font_COLOR);
	// Alignment
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_username_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_points_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_current_bet_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_action_LABEL, TRUE, TRUE, 0);
	// Card Creation & Alignment
	CGM_player_1_H_BOX = gtk_hbox_new(FALSE, 10);
	CGM_player_1_card_1 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	CGM_player_1_card_2 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_H_BOX, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_1_H_BOX), CGM_player_1_card_1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_1_H_BOX), CGM_player_1_card_2, TRUE, TRUE, 0);
	// Dealer Chip Creation & Alignment
	CGM_player_1_dealer_chip = create_dealer_chip();
	gtk_box_pack_start(GTK_BOX(CGM_player_1_V_BOX), CGM_player_1_dealer_chip, TRUE, TRUE, 0);


	GtkWidget *CGM_player_3_ALIGNMENT = NULL;
	GtkWidget *CGM_player_3_V_BOX = NULL;
	GtkWidget *CGM_player_3_H_BOX = NULL;
	// Create the 3rd player
	// Alignment
	CGM_player_3_ALIGNMENT = gtk_alignment_new(0.1, 0.9, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_left_V_BOX), CGM_player_3_ALIGNMENT, TRUE, TRUE, 0);
	CGM_player_3_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_player_3_ALIGNMENT), CGM_player_3_V_BOX);
	// Label Creation
	CGM_player_3_username_LABEL = create_placeholder_label("Player 3: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_3_points_LABEL = create_placeholder_label("Points: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_3_current_bet_LABEL = create_placeholder_label("Current Bet: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_3_action_LABEL = create_placeholder_label("Action: PLACEHOLDER", Client_white_font_COLOR);
	// Alignment
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_username_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_points_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_current_bet_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_action_LABEL, TRUE, TRUE, 0);
	// Card Creation & Alignment
	CGM_player_3_H_BOX = gtk_hbox_new(FALSE, 10);
	CGM_player_3_card_1 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	CGM_player_3_card_2 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_H_BOX, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_3_H_BOX), CGM_player_3_card_1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_3_H_BOX), CGM_player_3_card_2, TRUE, TRUE, 0);
	// Dealer Chip Creation & Alignment
	CGM_player_3_dealer_chip = create_dealer_chip();
	gtk_box_pack_start(GTK_BOX(CGM_player_3_V_BOX), CGM_player_3_dealer_chip, TRUE, TRUE, 0);

	GtkWidget *CGM_player_2_ALIGNMENT = NULL;
	GtkWidget *CGM_player_2_V_BOX = NULL;
	GtkWidget *CGM_player_2_H_BOX = NULL;
	// Create the 2st player
	// Alignment
	CGM_player_2_ALIGNMENT = gtk_alignment_new(0.9, 0.1, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_right_V_BOX), CGM_player_2_ALIGNMENT, TRUE, TRUE, 0);
	CGM_player_2_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_player_2_ALIGNMENT), CGM_player_2_V_BOX);
	// Label Creation
	CGM_player_2_username_LABEL = create_placeholder_label("Player 2: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_2_points_LABEL = create_placeholder_label("Points: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_2_current_bet_LABEL = create_placeholder_label("Current Bet: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_2_action_LABEL = create_placeholder_label("Action: PLACEHOLDER", Client_white_font_COLOR);
	// Alignment
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_username_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_points_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_current_bet_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_action_LABEL, TRUE, TRUE, 0);
	// Card Creation & Alignment
	CGM_player_2_H_BOX = gtk_hbox_new(FALSE, 10);
	CGM_player_2_card_1 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	CGM_player_2_card_2 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_H_BOX, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_2_H_BOX), CGM_player_2_card_1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_2_H_BOX), CGM_player_2_card_2, TRUE, TRUE, 0);
	// Dealer Chip Creation & Alignment
	CGM_player_2_dealer_chip = create_dealer_chip();
	gtk_box_pack_start(GTK_BOX(CGM_player_2_V_BOX), CGM_player_2_dealer_chip, TRUE, TRUE, 0);


	GtkWidget *CGM_player_4_ALIGNMENT = NULL;
	GtkWidget *CGM_player_4_V_BOX = NULL;
	GtkWidget *CGM_player_4_H_BOX = NULL;
	// Create the 4st player
	// Alignment
	CGM_player_4_ALIGNMENT = gtk_alignment_new(0.9, 0.9, 0, 0);
	gtk_box_pack_start(GTK_BOX(CGM_right_V_BOX), CGM_player_4_ALIGNMENT, TRUE, TRUE, 0);
	CGM_player_4_V_BOX = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(CGM_player_4_ALIGNMENT), CGM_player_4_V_BOX);
	// Label Creation
	CGM_player_4_username_LABEL = create_placeholder_label("Player 4: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_4_points_LABEL = create_placeholder_label("Points: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_4_current_bet_LABEL = create_placeholder_label("Current Bet: PLACEHOLDER", Client_white_font_COLOR);
	CGM_player_4_action_LABEL = create_placeholder_label("Action: PLACEHOLDER", Client_white_font_COLOR);
	// Alignment
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_username_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_points_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_current_bet_LABEL, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_action_LABEL, TRUE, TRUE, 0);
	// Card Creation & Alignment
	CGM_player_4_H_BOX = gtk_hbox_new(FALSE, 10);
	CGM_player_4_card_1 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	CGM_player_4_card_2 = create_poker_card("gui_images/playing_cards/back_of_card.png");
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_H_BOX, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_4_H_BOX), CGM_player_4_card_1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(CGM_player_4_H_BOX), CGM_player_4_card_2, TRUE, TRUE, 0);
	// Dealer Chip Creation & Alignment
	CGM_player_4_dealer_chip = create_dealer_chip();
	gtk_box_pack_start(GTK_BOX(CGM_player_4_V_BOX), CGM_player_4_dealer_chip, TRUE, TRUE, 0);


	/* Signal Connections */
	// Window Signal Connections
	// When the x button is clicked closes the window
	g_signal_connect(G_OBJECT(Client_WINDOW), "delete_event", G_CALLBACK(delete_event), NULL);
	// After the delete returns false a destroy event occurs and pass control is then passed back to main
	g_signal_connect(G_OBJECT(Client_WINDOW), "destroy", G_CALLBACK(destroy), NULL);
	// Main Menu Signal Connections
	// Update output_username whenever the entry box has been changed
    g_signal_connect(CMM_ENTRY_username, "changed", G_CALLBACK(update_output_username), output_username);
	// Update output_password whenever the entry box has been changed
    g_signal_connect(CMM_ENTRY_password, "changed", G_CALLBACK(update_output_password), output_password);
	// Update the seat number whenever the combo box has been changed
    g_signal_connect(CMM_COMBO_seat_dropdown, "changed", G_CALLBACK(update_output_seat_number), &output_seat_number);
	// Save user inputs and start the game once the button is clicked
	g_signal_connect(G_OBJECT(CMM_BUTTON_play), "clicked", G_CALLBACK(CMM_BUTTON_play_clicked), &menu_state);
	// Game Menu Signal Connections
	g_signal_connect(G_OBJECT(CGM_BUTTON_fold), "clicked", G_CALLBACK(CGM_BUTTON_fold_clicked), NULL); 
	g_signal_connect(G_OBJECT(CGM_BUTTON_call), "clicked", G_CALLBACK(CGM_BUTTON_call_clicked), NULL); 
	g_signal_connect(G_OBJECT(CGM_BUTTON_raise), "clicked", G_CALLBACK(CGM_BUTTON_raise_clicked), NULL); 
	g_signal_connect(G_OBJECT(CGM_BUTTON_all_in), "clicked", G_CALLBACK(CGM_BUTTON_all_in_clicked), NULL); 

	// Show widgets through Xming
	gtk_widget_show_all(Client_WINDOW);

	return Client_WINDOW;
}

void CGM_BUTTON_fold_clicked(GtkWidget *widget, gpointer data)
{
	output_fold = 1;
}

void CGM_BUTTON_call_clicked(GtkWidget *widget, gpointer data)
{
	output_call = 1;
}

void CGM_BUTTON_raise_clicked(GtkWidget *widget, gpointer data)
{
	// Copy the input from the username entry
	output_raise = atoi(gtk_entry_get_text((GtkEntry *)CMM_ENTRY_raise));
	printf("Current raise: %d\n", output_raise);
}

void CGM_BUTTON_all_in_clicked(GtkWidget *widget, gpointer data)
{
	output_all_in = 1;
}

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("Delete event occurred!\n");
	g_print ("Closing Xming Window!\n");
	gtk_widget_destroy(widget);
    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event". */
    return FALSE;
}

void destroy(GtkWidget *widget, gpointer data)
{
	Client_Window_CLOSED = TRUE;
    // gtk_main_quit();
}

void update_output_seat_number(GtkComboBox *CMM_COMBO_seat_dropdown, int *client_seat)
{
	// Copy the seat number from the new selected text
	int number_assigned = 0;
	number_assigned = sscanf(gtk_combo_box_get_active_text(CMM_COMBO_seat_dropdown), "Seat %d", client_seat);
	assert(number_assigned == 1);
	printf("Current Seat: %d\n", *(client_seat));

	// Fatal error goes here as sscanf is unsuccessful in copying the int from the text
	// if(number_assigned != 1)
}

void CMM_BUTTON_play_clicked(GtkWidget *widget, gpointer data)
{
	// Update game state
	int *value_ptr = (int *)data;
	*(value_ptr) = 1;
	
	// Go to the game menu
	gtk_notebook_set_current_page(GTK_NOTEBOOK(Client_NOTEBOOK), 1);
	printf("Moved to game!\n");
}

void get_card_file_path(char *card_in_string_format, int size_of_card_in_string_format)
{
	// Temporary Variable
	char temp_file_path[STRING_BUFFER];
	// Create the string filepath and store it in the temp
	snprintf(temp_file_path, sizeof(temp_file_path),"gui_images/playing_cards/%s.png", card_in_string_format);
	// Set the original string pointer to the newly created string filepath
	snprintf(card_in_string_format, size_of_card_in_string_format, "%s", temp_file_path);

	// // Variables
	// char file_path[STRING_BUFFER] = "gui_images/playing_cards/";
	// char png[STRING_BUFFER] = ".png";
	// // Create the file path for the type of playing card
	// strcat(file_path, card_in_string_format);
	// strcat(file_path, png);
	// // Copy the file path back to the original string array
	// snprintf(card_in_string_format, STRING_BUFFER, "%s", file_path);
}

GtkWidget *create_poker_card(char *poker_card_file_path)
{
	// Variables
	GdkPixbuf *poker_card = NULL;
    GtkWidget *image = NULL;

    // Load the image into a the poker_card GdkPixbuf
    poker_card = gdk_pixbuf_new_from_file(poker_card_file_path, NULL);
    if (!poker_card)
	{
		// Prints error to standard error stream
        fprintf(stderr, "The poker card image can't be loaded!\n");
		poker_card = gdk_pixbuf_new_from_file("gui_images/playing_cards/back_of_card.png", NULL);
	}
	// Resize the poker_card to the set card size
	poker_card = gdk_pixbuf_scale_simple(poker_card, CARD_WIDTH, CARD_HEIGHT, GDK_INTERP_BILINEAR);
	// Create a GtkImage widget with the resized pixbuf
	image = gtk_image_new_from_pixbuf(poker_card);
	// Free the pixbuf as it's no longer needed
	g_object_unref(poker_card); 
	// Return the GTK image
	return image;
}

void update_output_username(GtkEntry *CMM_ENTRY_username, char *output_username)
{
	// Copy the input from the username entry
	int number_assigned = 0;
	number_assigned = snprintf(output_username, STRING_BUFFER, "%s", gtk_entry_get_text(CMM_ENTRY_username));
	assert(number_assigned >= 0 && number_assigned <= STRING_BUFFER - 1);
	printf("Current username: %s\n", output_username);
}

void update_output_password(GtkEntry *CMM_ENTRY_password, char *output_password)
{
	// Copy the input from the password entry
	int number_assigned = 0;
	number_assigned = snprintf(output_password, STRING_BUFFER, "%s", gtk_entry_get_text(CMM_ENTRY_password));
	assert(number_assigned >= 0 && number_assigned <= STRING_BUFFER - 1);
	printf("Current password: %s\n", output_password);
}

void update_window_settings(GtkWidget *Client_WINDOW)
{	
	// Variable
	GdkPixbuf *pokerIcon = NULL;

	// Set defualt window settings for the GUI window
	gtk_window_set_title(GTK_WINDOW(Client_WINDOW), "Poker");
	gtk_window_set_default_size(GTK_WINDOW(Client_WINDOW), WINDOW_WIDTH, WINDOW_HEIGHT);
	// Center GTK window
	gtk_window_set_position(GTK_WINDOW(Client_WINDOW), GTK_WIN_POS_CENTER);
	// gtk_window_set_resizable(GTK_WINDOW(Client_WINDOW), FALSE);
	// gtk_container_set_border_width(GTK_CONTAINER(Client_WINDOW), 10);
	
	// Add poker icon to window
	pokerIcon = gdk_pixbuf_new_from_file("gui_images/PokerIcon.png", NULL);
	if (!pokerIcon)
	{
		// Prints error to standard error stream
		fprintf(stderr, "The poker icon image can't be loaded!\n");
	}
  	gtk_window_set_icon(GTK_WINDOW(Client_WINDOW), pokerIcon);
	g_object_unref(pokerIcon);
}

void update_dealer_message_LABEL(char *input_value)
{
	// Variable
	char buffer[STRING_BUFFER];

    // Make sure the label exists first
    if (Dealer_Message_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
		// printf("%d\n",input_value);
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Dealer: %s</span>", input_value);
        gtk_label_set_markup(GTK_LABEL(Dealer_Message_LABEL), buffer);
    }
}

void update_side_pot_LABEL(int input_value)
{
	// Variable
	char buffer[STRING_BUFFER];

    // Make sure the label exists first
    if (side_pot_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
		// printf("%d\n",input_value);
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Side Pot: %d</span>", input_value);
        gtk_label_set_markup(GTK_LABEL(side_pot_LABEL), buffer);
    }
}

void update_dealer_chip(int INPUT_dealer_chip[4])
{

	if (INPUT_dealer_chip[0] != 1)
	{
		gtk_widget_hide(CGM_player_1_dealer_chip);
	}
	else
	{
		gtk_widget_show(CGM_player_1_dealer_chip);
	}
	if (INPUT_dealer_chip[1] != 1)
	{
		gtk_widget_hide(CGM_player_2_dealer_chip);
	}
	else
	{
		gtk_widget_show(CGM_player_2_dealer_chip);
	}
	if (INPUT_dealer_chip[2] != 1)
	{
		gtk_widget_hide(CGM_player_3_dealer_chip);
	}
	else
	{
		gtk_widget_show(CGM_player_3_dealer_chip);
	}
	if (INPUT_dealer_chip[3] != 1)
	{
		gtk_widget_hide(CGM_player_4_dealer_chip);
	}
	else
	{
		gtk_widget_show(CGM_player_4_dealer_chip);
	}
}

void update_poker_card(GtkWidget* image_to_be_updated, char *input_poker_card)
{
	// Variables
	GdkPixbuf *poker_card = NULL;
	// Get Pixbuff and resize it
	poker_card = gdk_pixbuf_new_from_file(input_poker_card, NULL);
	if (!poker_card)
	{
		// Prints error to standard error stream
        fprintf(stderr, "The poker card image can't be loaded!\n");
		poker_card = gdk_pixbuf_new_from_file("gui_images/playing_cards/back_of_card.png", NULL);
	}
	// Resize the poker_card to the set card size
	poker_card = gdk_pixbuf_scale_simple(poker_card, CARD_WIDTH, CARD_HEIGHT, GDK_INTERP_BILINEAR);
	// Set existing image to the new pixbuff
	gtk_image_set_from_pixbuf(GTK_IMAGE(image_to_be_updated), poker_card);	
}

void update_player_cards(char INPUT_player_1_cards[2][STRING_BUFFER], char INPUT_player_2_cards[2][STRING_BUFFER], char INPUT_player_3_cards[2][STRING_BUFFER], char INPUT_player_4_cards[2][STRING_BUFFER])
{
	// Player 1
	get_card_file_path(INPUT_player_1_cards[0], sizeof(INPUT_player_1_cards[0]));
	update_poker_card(CGM_player_1_card_1, INPUT_player_1_cards[0]);
	get_card_file_path(INPUT_player_1_cards[1], sizeof(INPUT_player_1_cards[1]));
	update_poker_card(CGM_player_1_card_2, INPUT_player_1_cards[1]);
	// Player 2
	get_card_file_path(INPUT_player_2_cards[0], sizeof(INPUT_player_2_cards[0]));
	update_poker_card(CGM_player_2_card_1, INPUT_player_2_cards[0]);
	get_card_file_path(INPUT_player_2_cards[1], sizeof(INPUT_player_2_cards[1]));
	update_poker_card(CGM_player_2_card_2, INPUT_player_2_cards[1]);
	// Player 3
	get_card_file_path(INPUT_player_3_cards[0], sizeof(INPUT_player_3_cards[0]));
	update_poker_card(CGM_player_3_card_1, INPUT_player_3_cards[0]);
	get_card_file_path(INPUT_player_3_cards[1], sizeof(INPUT_player_3_cards[1]));
	update_poker_card(CGM_player_3_card_2, INPUT_player_3_cards[1]);
	// Player 4
	get_card_file_path(INPUT_player_4_cards[0], sizeof(INPUT_player_4_cards[0]));
	update_poker_card(CGM_player_4_card_1, INPUT_player_4_cards[0]);
	get_card_file_path(INPUT_player_4_cards[1], sizeof(INPUT_player_4_cards[1]));
	update_poker_card(CGM_player_4_card_2, INPUT_player_4_cards[1]);
}

void update_dealt_cards(char INPUT_dealt_cards[5][STRING_BUFFER])
{
	for (int i = 0; i < 5; i++)
	{
		get_card_file_path(INPUT_dealt_cards[i], sizeof(INPUT_dealt_cards[i]));
	}
	update_poker_card(CGM_dealt_card_IMAGE_1, INPUT_dealt_cards[0]);
	update_poker_card(CGM_dealt_card_IMAGE_2, INPUT_dealt_cards[1]);
	update_poker_card(CGM_dealt_card_IMAGE_3, INPUT_dealt_cards[2]);
	update_poker_card(CGM_dealt_card_IMAGE_4, INPUT_dealt_cards[3]);
	update_poker_card(CGM_dealt_card_IMAGE_5, INPUT_dealt_cards[4]);
}

void update_action_LABELs(int INPUT_player_action[4])
{
	// Variable
	char buffer[STRING_BUFFER];
    // Make sure the label exists first
    if (CGM_player_1_action_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Action: %d</span>", INPUT_player_action[0]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_1_action_LABEL), buffer);
    }
	if (CGM_player_2_action_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Action: %d</span>", INPUT_player_action[1]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_2_action_LABEL), buffer);
    }
	if (CGM_player_3_action_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Action: %d</span>", INPUT_player_action[2]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_3_action_LABEL), buffer);
    }
	if (CGM_player_4_action_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Action: %d</span>", INPUT_player_action[3]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_4_action_LABEL), buffer);
    }
}

void update_current_bet_LABELs(int INPUT_player_current_bet[4])
{
	// Variable
	char buffer[STRING_BUFFER];
    // Make sure the label exists first
    if (CGM_player_1_current_bet_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Current Bet: %d</span>", INPUT_player_current_bet[0]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_1_current_bet_LABEL), buffer);
    }
	if (CGM_player_2_current_bet_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Current Bet: %d</span>", INPUT_player_current_bet[1]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_2_current_bet_LABEL), buffer);
    }
	if (CGM_player_3_current_bet_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Current Bet: %d</span>", INPUT_player_current_bet[2]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_3_current_bet_LABEL), buffer);
    }
	if (CGM_player_4_current_bet_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Current Bet: %d</span>", INPUT_player_current_bet[3]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_4_current_bet_LABEL), buffer);
    }
}

void update_points_LABELs(int INPUT_player_points[4])
{
	// Variable
	char buffer[STRING_BUFFER];
    // Make sure the label exists first
    if (CGM_player_1_points_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Points: %d</span>", INPUT_player_points[0]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_1_points_LABEL), buffer);
    }
	if (CGM_player_2_points_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Points: %d</span>", INPUT_player_points[1]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_2_points_LABEL), buffer);
    }
	if (CGM_player_3_points_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Points: %d</span>", INPUT_player_points[2]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_3_points_LABEL), buffer);
    }
	if (CGM_player_4_points_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Points: %d</span>", INPUT_player_points[3]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_4_points_LABEL), buffer);
    }
}

void update_username_LABELs(char INPUT_player_usernames[4][STRING_BUFFER])
{
	// Variable
	char buffer[STRING_BUFFER];
    // Make sure the label exists first
    if (CGM_player_1_username_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Player 1: %s</span>", INPUT_player_usernames[0]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_1_username_LABEL), buffer);
    }
	if (CGM_player_2_username_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Player 1: %s</span>", INPUT_player_usernames[1]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_2_username_LABEL), buffer);
    }
	if (CGM_player_3_username_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Player 1: %s</span>", INPUT_player_usernames[2]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_3_username_LABEL), buffer);
    }
	if (CGM_player_4_username_LABEL != NULL && gtk_notebook_get_current_page(GTK_NOTEBOOK(Client_NOTEBOOK)) == 1)
	{
        snprintf(buffer, sizeof(buffer), "<span size='xx-large'>Player 1: %s</span>", INPUT_player_usernames[3]);
        gtk_label_set_markup(GTK_LABEL(CGM_player_4_username_LABEL), buffer);
    }
}

GtkWidget *create_dealer_chip(void)
{
	// Variables
	GdkPixbuf *poker_chip = NULL;
    GtkWidget *image = NULL;

    // Load the image into a the poker_chip GdkPixbuf
    poker_chip = gdk_pixbuf_new_from_file("gui_images/dealer_chip.png", NULL);
	if (poker_chip)
	{
		// Resize the poker_chip to the set chip size
		poker_chip = gdk_pixbuf_scale_simple(poker_chip, CHIP_WIDTH, CHIP_HEIGHT, GDK_INTERP_BILINEAR);
		// Create a GtkImage widget with the resized pixbuf
		image = gtk_image_new_from_pixbuf(poker_chip);
		// Free the pixbuf as it's no longer needed
		g_object_unref(poker_chip); 
		// Return the GTK image
		return image;
	}
	else {
		// Prints error to standard error stream
        fprintf(stderr, "The poker chip image can't be loaded!\n");
		// image = gtk_image_new();
		// // Free the pixbuf as it's no longer needed
		// g_object_unref(poker_chip); 
		// // Return the GTK image
		// return image;
		image = gtk_frame_new(NULL);
		gtk_widget_set_size_request(image, CARD_WIDTH, CARD_HEIGHT);
		gtk_frame_set_shadow_type(GTK_FRAME(image), GTK_SHADOW_NONE);
    	// gtk_frame_set_shadow_type(GTK_FRAME(image), GTK_SHADOW_IN);
        return image;
	}
}

GtkWidget *create_placeholder_label(char *placeholder_text, GdkColor font_COLOR)
{
	// Variable
	GtkWidget *placeholder_LABEL = NULL;
	char buffer[STRING_BUFFER];

	// Create the "Placeholder" label
	placeholder_LABEL = gtk_label_new(NULL);
	snprintf(buffer, sizeof(buffer), "<span size='xx-large'>%s</span>", placeholder_text);
	gtk_label_set_markup(GTK_LABEL(placeholder_LABEL), buffer);
	gtk_widget_modify_fg(placeholder_LABEL, GTK_STATE_NORMAL, &font_COLOR);
	gtk_label_set_line_wrap(GTK_LABEL(placeholder_LABEL), TRUE);
	gtk_label_set_justify(GTK_LABEL(placeholder_LABEL), GTK_JUSTIFY_CENTER);
	
	// Return the created "Placeholder" label
	return placeholder_LABEL;
}








/* NOT WORKING */
/*
// GdkPixbuf *create_pixbuf(const gchar * filename) {
    
//    GdkPixbuf *pixbuf;
//    GError *error = NULL;
//    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
//    if (!pixbuf) {
       
//       fprintf(stderr, "%s\n", error->message);
//       g_error_free(error);
//    }

//    return pixbuf;
// }


// update_background_image is connected whenever the window is resized remaking the background effectively making the background resize as well
g_signal_connect(G_OBJECT(Client_WINDOW), "size-request", G_CALLBACK(update_background_image), Client_notebook_PAGE_1);
void update_background_image(GtkWidget *widget, gpointer data)
{
	// NOT WORKING
	int window_width = WINDOW_WIDTH;
	int window_height = WINDOW_HEIGHT;
	// Get the size of the window
    gtk_window_get_size(GTK_WINDOW(widget), &window_width, &window_height);

	
	// set_event_box_background_image(event_box, "gui_images/CMM_BG_2.jpeg");
	printf("Window resized: Width=%d, Height=%d\n", window_width, window_height);
	GtkWidget *event_box = (GtkWidget *)(data);
	set_event_box_background_image(event_box, "gui_images/CMM_BG.jpeg");
}

char *get_card_file_path(char *card_in_string_format)
{
	// Variables
	char card_rank[STRING_BUFFER];
	char card_suit[STRING_BUFFER];
	// Convert the string formated card into it's suit and rank
	sscanf(card_in_string_format, "%99s of %99s", card_rank, card_suit);
	// card_rank[0] = toUpperCase(card_rank[0]);
	// card_suit[0] = toUpperCase(card_suit[0]);
	printf("Card Rank: %s\tSuit: %s\n", card_rank, card_suit);
	strcat(card_rank,card_suit);
	printf("Test: %s\n", card_rank);
	
	// if (strcmp(card_suit, "Clubs") == 0)
	// {
	// 	printf("Clubs\n");
	// 	if (strcmp(card_suit, "Ace") == 0)
	// 	{
	// 		return "gui_images/playing_cards/ace_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "2") == 0)
	// 	{
	// 		return "gui_images/playing_cards/2_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "3") == 0)
	// 	{
	// 		return "gui_images/playing_cards/3_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "4") == 0)
	// 	{
	// 		return "gui_images/playing_cards/4_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "5") == 0)
	// 	{
	// 		return "gui_images/playing_cards/5_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "6") == 0)
	// 	{
	// 		return "gui_images/playing_cards/6_of_clubs.png";
	// 	}
	// 	else if (strcmp(card_suit, "7") == 0)
	// 	{
	// 		return "gui_images/playing_cards/7_of_clubs.png";
	// 	}
	// }
	// else if (strcmp(card_suit, "Diamonds") == 0)
	// {
	// 	printf("Diamonds\n");
	// }
	// else if (strcmp(card_suit, "Hearts") == 0)
	// {
	// 	printf("Hearts\n");
	// }
	// else if (strcmp(card_suit, "Spades") == 0)
	// {
	// 	printf("Spades\n");
	// }
	return "N/A";
}


// // Create a label that displays the ASCII main menu
// CMM_menu_LABEL = gtk_label_new("Main Menu");
// gtk_label_set_line_wrap(GTK_LABEL(CMM_menu_LABEL), TRUE);
// gtk_label_set_justify(GTK_LABEL(CMM_menu_LABEL), GTK_JUSTIFY_CENTER);
// gtk_box_pack_start(GTK_BOX(CMM_top_V_BOX), CMM_menu_LABEL, TRUE, TRUE, 0);

// testing
// Create a card shadow
// function : create card shadows
GtkWidget *FRAME_card_test = gtk_frame_new(NULL);
gtk_frame_set_shadow_type(GTK_FRAME(FRAME_card_test), GTK_SHADOW_IN);
gtk_box_pack_start(GTK_BOX(CGM_middleV_BOX), FRAME_card_test, TRUE, TRUE, 0);
gtk_container_remove(GTK_CONTAINER(CGM_middleV_BOX), FRAME_card_test);

// Update the background image whenever the window is resized
// NOT WORKING
// g_signal_connect(G_OBJECT(Client_WINDOW), "configure-event", G_CALLBACK(update_background_image), Client_notebook_PAGE_1);

// Copy the username from the username entry box into a character array to be used later
snprintf(output_username, sizeof(output_username),"%s",gtk_entry_get_text((GtkEntry *)CMM_ENTRY_username));
// Copy the password from the password entry box into a character array to be used later
snprintf(output_password, sizeof(output_password),"%s",gtk_entry_get_text((GtkEntry *)CMM_ENTRY_password));

*/