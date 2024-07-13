/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* unit_test_gamelogic.c: Unit test for gamelogic	                 */
/*********************************************************************/

#include "gamelogic.h"
#include <stdio.h>

// made a function to itterate through the deck and print


void printcurrentDeck(const Deck *deck) {
    for (int i = 0; i < NUM_CARDS; ++i) {
        printf("Card %d: Suit %d, Rank %d\n", i+1, deck->cards[i].suit, deck->cards[i].rank);
    }
}
/*
int main()
{
    Deck deck;
    Game game;
    Player player;

    int numPlayers = 4;
    int smallBlind = 10;
    int bigBlind = 20;
    int startingChips = 1000;

    //These are the deck functions
    makeDeck(&deck);
    printf("Printing deck: \n");
    printcurrentDeck(&deck);
    printf("These are the cards printed");

    //I lowkey dont know how to test this one through print statements 
    printDeck(&deck);
    displayCommunityCards(&deck);

    shuffleDeck(&deck);
    printf("Printing deck: \n");
    printcurrentDeck(&deck);
    printf("These are the cards printed");

    dealCards(&game);
    
    initGame(&game, numPlayers);
    startGame(&game);

    displayPlayerCards(&player);




}
*/

const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

int main() {
    int numPlayers;

    // Ask for the number of players
    printf("Enter the number of players (2-4): ");
    scanf("%d", &numPlayers);

    if (numPlayers < 2 || numPlayers > 4) {
        printf("Invalid number of players. Please restart the game and enter a valid number of players.\n");
        return 1;
    }

    // Initialize the game (This includes stuff like makeDeck, shuffleDeck, dealCards)
    Game game;
    initGame(&game, numPlayers);

    // Start the game (itterates through rounds)
    startGame(&game);

    // Determine the winner
    int winner = Winner(&game);
    if (winner == -1) {
        printf("It's a tie!\n");
        Tie(&game);
    } else {
        printf("Player %d wins!\n", winner);
    }

    return 0;
}

