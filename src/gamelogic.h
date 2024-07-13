#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <stdlib.h>
#include <stdbool.h>

#define NUM_CARDS 52

typedef enum{
    SPADES = 0,
    CLUBS = 1,
    DIAMONDS = 2,
    HEARTS = 3
}SUIT;

typedef enum{
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    ACE = 14
}RANK;

typedef enum {
    PREFLOP = 0,
    FLOP = 1,
    TURN = 2,
    RIVER = 3
}ROUND;

typedef enum {
    SMALL_BLIND = 0,
    BIG_BLIND = 1,
    REGULAR = 2
}BLIND;

typedef struct {
	SUIT suit;
	RANK rank;
}Card;

typedef struct {
	Card cards[NUM_CARDS];
	int top;
}Deck;

typedef enum {
    FOLD = 0,
    CHECK = 1,
    CALL = 2,
    RAISE = 3
}Moves;

typedef struct {
    int chips;
    int bet;
    int raise;
    Card card1;
    Card card2;
    Moves move;
}Player;

//create a struct for the state of the game
typedef struct {
    int pot;
    ROUND round;
    int currentCall;
    int numPlayers;
    int currentPlayer;
    int numGames;
    int dealer;
    int smallBlind;
    int bigBlind;
    int raise;
    int maxBet;
    int minBet;
    int maxRaise;
    int minRaise;
    int numFolded;
    int numCalled;
    int numRaised;
    int numChecked;
    int numAllIn;
    Player players[4];
    Deck shuffleDeck;
    Deck communityCards;
    int playerTurn;
}Game;

extern const char *suits[];
extern const char *ranks[];

void makeDeck(Deck *deck);
void shuffleDeck(Deck *deck);
void printDeck(Deck *deck);
void dealCards(Game *game);
int EqualBids(Game *game);
int Winner (Game *game);
Game *Tie (Game *game);
int getMaxPriorityOfThePlayer(Game *game, int person);
int CheckPlayer(Game *game, int player);
int CheckRoyalFlush(Game *game, int player);
int CheckStraightFlush(Game *game, int player);
int CheckFourOfAKind(Game *game, int player);
int CheckFullHouse(Game *game, int player);
int CheckFlush(Game *game, int player);
int CheckStraight(Game *game, int player);
int CheckThreeOfAKind(Game *game, int player);
int CheckTwoPair(Game *game, int player);
int CheckPair(Game *game, int player);
int CheckHighCard(Game *game, int player);
Deck* SortbyRank(Deck *D);
Deck* SortbySuit(Deck *D);
void initGame(Game *game, int numPlayers);
void playerAction(Game *game, int playerIndex);
void startGame(Game *game);
void displayPlayerCards(Player *player);
void displayCommunityCards(Deck *communityCards);
int findHighestBet(Game *game);



#endif // POKER_H

