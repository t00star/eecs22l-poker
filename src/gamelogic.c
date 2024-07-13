/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* gamelogic.c: source file for the poker game logic				 */
/*********************************************************************/

#include "gamelogic.h"
#include "time.h"
#include <stdio.h>
#include <limits.h>

const char *suits[1];
const char *ranks[1];

void makeDeck(Deck *deck){
    int count = 0;
    for (int suit = 0; suit < 4; suit++)
    {
        for (int rank = 2; rank < 15; rank++)
        {
            deck->cards[count].suit = suit;
            deck->cards[count].rank = rank;
            count++;
        }
    }
}

void shuffleDeck(Deck *deck)
{
    srand(time(NULL));
    for (int i = NUM_CARDS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

void printDeck(Deck *deck)
{
    for (int i = 0; i < NUM_CARDS; i++)
    {
        printf("Card %d: Suit %d, Rank %d\n", i+1, deck->cards[i].suit, deck->cards[i].rank);
    }
}

void dealCards(Game *game)
{
    int deckIndex = 0;
    for (int i = 0; i < game->numPlayers; i++)
    {
        game->players[i].card1 = game->shuffleDeck.cards[deckIndex++];
        game->players[i].card2 = game->shuffleDeck.cards[deckIndex++];
    }

    for (int i = 0; i < 5; i++)
    {
        game->communityCards.cards[i] = game->shuffleDeck.cards[deckIndex++];
    }
    game->communityCards.top = 5;
}

int EqualBids(Game *game){
    int currentBid;
    int firstPlayer;

    //find the first player in play and record their bet
    for (int i = 0; i < game->numPlayers; i++){
        if (game->players[game->playerTurn].move != FOLD){
            firstPlayer = i;
            currentBid = game->players[i].bet;
            break;
        }
    }

    //check if all players have the same bid
    for (int i = firstPlayer + 1; i < game->numPlayers; i++){
        if (game->players[game->playerTurn].move != FOLD){
            if (game->players[i].bet != currentBid){
                return 0;
            }
        }
    }

    return 1;
}

int Winner (Game *game){
    int winner = 0;
    int highestPriority = 0;
    for (int i = 0; i < game->numPlayers; ++i){
        int priority = CheckPlayer(game, i);
        if (priority > highestPriority){
            highestPriority = priority;
            winner = i;
        }
        else if (priority == highestPriority){
                winner = -1;
            
        }
    }
    return winner;
}

Game *Tie (Game *game){
    //find the priorities of each of the player
    int priorities[4];
    for (int i = 0; i < game->numPlayers; i++){
        priorities[i] = CheckPlayer(game, i);
    }

    //pick the player with the highest priority
    int maxPriorityValue;
    int PlayersWhoTie = 0;
    for (int i = 0; i < game->numPlayers; i++){
        if (priorities[i] > maxPriorityValue){
            maxPriorityValue = priorities[i];
        }
    }

    //find number of players who tied
    for (int i = 0; i < game->numPlayers; i++){
        if (priorities[i] == maxPriorityValue){
            PlayersWhoTie++;
        }
    }
    
    //index for tied players
    int tiedPlayers[4];
    int j = 0;
        for (int i = 0; i < game->numPlayers; i++){
            if (priorities[i] == maxPriorityValue){
                tiedPlayers[j] = i;
                j++;
            }
        }

    //split the pot among these players
    int potToDistribute = game->pot / PlayersWhoTie;
    for (int i = 0; i < PlayersWhoTie; i++){
        game->players[tiedPlayers[i]].chips += potToDistribute;
    }

    return game;
}

int getMaxPriorityOfThePlayer(Game *game, int person){
    int max = INT_MIN;
    max = (max > CheckRoyalFlush(game, person)) ? max : CheckRoyalFlush(game, person);
    max = (max > CheckStraightFlush(game, person)) ? max : CheckStraightFlush(game, person);
    max = (max > CheckFourOfAKind(game, person)) ? max : CheckFourOfAKind(game, person);
    max = (max > CheckFullHouse(game, person)) ? max : CheckFullHouse(game, person);
    max = (max > CheckFlush(game, person)) ? max : CheckFlush(game, person);
    max = (max > CheckStraight(game, person)) ? max : CheckStraight(game, person);
    max = (max > CheckThreeOfAKind(game, person)) ? max : CheckThreeOfAKind(game, person);
    max = (max > CheckTwoPair(game, person)) ? max : CheckTwoPair(game, person);
    max = (max > CheckPair(game, person)) ? max : CheckPair(game, person);
    max = (max > CheckHighCard(game, person)) ? max : CheckHighCard(game, person);
    return max;
}

int CheckPlayer(Game *game, int player){
    int priority;
    if(CheckRoyalFlush(game, player) == 1){
        priority = 10;
    }
    else if(CheckStraightFlush(game, player) == 1){
        priority = 9;
    }
    else if(CheckFourOfAKind(game, player) == 1){
        priority = 8;
    }
    else if(CheckFullHouse(game, player) == 1){
        priority = 7;
    }
    else if(CheckFlush(game, player) == 1){
        priority = 6;
    }
    else if(CheckStraight(game, player) == 1){
        priority = 5;
    }
    else if(CheckThreeOfAKind(game, player) == 1){
        priority = 4;
    }
    else if(CheckTwoPair(game, player) == 1){
        priority = 3;
    }
    else if(CheckPair(game, player) == 1){
        priority = 2;
    }
    else{
        priority = CheckHighCard(game, player);
    }

    return priority;
}

int CheckRoyalFlush(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    if (NewDeck == NULL){
        return 0;
    }

    int a, b, c, d, e, f, g, i;

    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }

    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    NewDeck = SortbyRank(NewDeck);

    a = NewDeck->cards[0].rank;
    b = NewDeck->cards[1].rank;
    c = NewDeck->cards[2].rank;
    d = NewDeck->cards[3].rank;
    e = NewDeck->cards[4].rank;
    f = NewDeck->cards[5].rank;
    g = NewDeck->cards[6].rank;

    if (CheckFlush(game, player) == 1 && CheckStraight(game, player) == 1 && c == 10 && d == 11 && e == 12 && f == 13 && g == 14){
        return 1;
    }
    if (CheckFlush(game, player) == 1 && CheckStraight(game, player) == 1 && b == 10 && c == 11 && d == 12 && e == 13 && f == 14){
        return 1;
    }
    if(CheckFlush(game, player) == 1 && CheckStraight(game, player) == 1 && a == 10 && b == 11 && c == 12 && d == 13 && e == 14){
        return 1;
    }
    else{
        return 0;
    }

    free(NewDeck);
}

int CheckStraightFlush(Game *game, int player){
    if (CheckFlush(game, player) == 1 && CheckStraight(game, player) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int CheckFourOfAKind(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;
    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    NewDeck = SortbyRank(NewDeck);

    a = NewDeck->cards[0].rank;
    b = NewDeck->cards[1].rank;
    c = NewDeck->cards[2].rank;
    d = NewDeck->cards[3].rank;
    e = NewDeck->cards[4].rank;
    f = NewDeck->cards[5].rank;
    g = NewDeck->cards[6].rank;

    if ((a == b && b == c && c == d) || (b == c && c == d && d == e) || (c == d && d == e && e == f) || (d == e && e == f && f == g)){
        return 1;
    }
    else{
        return 0;
    }
    free(NewDeck);

}

int CheckFullHouse(Game *game, int player){
    if (CheckThreeOfAKind(game, player) == 1 && CheckPair(game, player) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int CheckFlush(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;
    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    NewDeck = SortbySuit(NewDeck);

    a = NewDeck->cards[0].suit;
    b = NewDeck->cards[1].suit;
    c = NewDeck->cards[2].suit;
    d = NewDeck->cards[3].suit;
    e = NewDeck->cards[4].suit;
    f = NewDeck->cards[5].suit;
    g = NewDeck->cards[6].suit;

    if ((a == b && b == c && c == d && d == e) || (b == c && c == d && d == e && e == f) || (c == d && d == e && e == f && f == g)){
        return 1;
    }
    else{
        return 0;
    }
    free(NewDeck);
}

int CheckStraight(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;
    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    NewDeck = SortbyRank(NewDeck);

    a = NewDeck->cards[0].rank;
    b = NewDeck->cards[1].rank;
    c = NewDeck->cards[2].rank;
    d = NewDeck->cards[3].rank;
    e = NewDeck->cards[4].rank;
    f = NewDeck->cards[5].rank;
    g = NewDeck->cards[6].rank;

    if ((e == d+1 && d == c+1 && c == b+1 && b == a+1) || (f == e+1 && e == d+1 && d == c+1 && c == b+1) || (g == f+1 && f == e+1 && e == d+1 && d == c+1)){
        return 1;
    }
    else{
        return 0;
    }
    free(NewDeck);
}

int CheckThreeOfAKind(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;
    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    NewDeck = SortbyRank(NewDeck);

    a = NewDeck->cards[0].rank;
    b = NewDeck->cards[1].rank;
    c = NewDeck->cards[2].rank;
    d = NewDeck->cards[3].rank;
    e = NewDeck->cards[4].rank;
    f = NewDeck->cards[5].rank;
    g = NewDeck->cards[6].rank;

    if ((a == b && b == c) || (b == c && c == d) || (c == d && d == e) || (d == e && e == f) || (e == f && f == g)){
        return 1;
    }
    else{
        return 0;
    }
    free(NewDeck);
}

int CheckTwoPair(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;
        for (i=0; i<5; i++){
            NewDeck->cards[i] = game->communityCards.cards[i];
        }
    
        //this is the player's hand
        NewDeck->cards[5] = game->players[player].card1;
        NewDeck->cards[6] = game->players[player].card2;
    
        //sort the deck by rank
        NewDeck = SortbyRank(NewDeck);
    
        a = NewDeck->cards[0].rank;
        b = NewDeck->cards[1].rank;
        c = NewDeck->cards[2].rank;
        d = NewDeck->cards[3].rank;
        e = NewDeck->cards[4].rank;
        f = NewDeck->cards[5].rank;
        g = NewDeck->cards[6].rank;

        //write if statements for all ways that the a-g can have two pairs
        if ((a == b && ( c == d || d == e || e == f || f == g)) || (b == c && (a == d || d == e || e == f || f == g)) || (c == d && (e == f || f == g)) || (d == e && (f == g))){
            return 1;
        }
        else{
            return 0;
        }
        free(NewDeck);
}

int CheckPair(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    int a, b, c, d, e, f, g, i;

    for (i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }

    //this is the player's hand
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    //sort the deck by rank
    NewDeck = SortbyRank(NewDeck);

    a = NewDeck->cards[0].rank;
    b = NewDeck->cards[1].rank;
    c = NewDeck->cards[2].rank;
    d = NewDeck->cards[3].rank;
    e = NewDeck->cards[4].rank;
    f = NewDeck->cards[5].rank;
    g = NewDeck->cards[6].rank;

    //check if the player has a pair
    if ((a == b) || (b == c) || (c == d) || (d == e) || (e == f) || (f == g)){
        return 1;
    }
    else{
        return 0;
    }
    free(NewDeck);
}

int CheckHighCard(Game *game, int player){
    Deck *NewDeck = malloc(sizeof(Deck));
    for (int i=0; i<5; i++){
        NewDeck->cards[i] = game->communityCards.cards[i];
    }

    //this is the player's hand
    NewDeck->cards[5] = game->players[player].card1;
    NewDeck->cards[6] = game->players[player].card2;

    //sort the deck by rank
    NewDeck = SortbyRank(NewDeck);

    int a = NewDeck->cards[6].rank;

    //check if the player has a high card

    if (a == 14){
        return 14;
    }
    if(a == 13){
        return 13;
    }
    if(a == 12){
        return 12;
    }
    if(a == 11){
        return 11;
    }
    if(a == 10){
        return 10;
    }
    if(a == 9){
        return 9;
    }
    if(a == 8){
        return 8;
    }
    if(a == 7){
        return 7;
    }
    if(a == 6){
        return 6;
    }
    if(a == 5){
        return 5;
    }
    if(a == 4){
        return 4;
    }
    if(a == 3){
        return 3;
    }
    if(a == 2){
        return 2;
    }
    return 0;
    free(NewDeck);
}


//sort rank by rank
Deck* SortbyRank(Deck *D){
    Card A;
    int i, j;
    for (i=0; i<7; i++){
        for (j=0; j<7-i-1; j++){
            if (D->cards[j].rank > D->cards[j+1].rank){
                A = D->cards[j];
                D->cards[j] = D->cards[j+1];
                D->cards[j+1] = A;
            }
        }
    }
    return D;
}

//Sort by suit
Deck* SortbySuit(Deck *D){
    Card A;
    int i, j;
    for (i=0; i<7; i++){
        for (j=0; j<7-i-1; j++){
            if (D->cards[j].suit > D->cards[j+1].suit){
                A = D->cards[j];
                D->cards[j] = D->cards[j+1];
                D->cards[j+1] = A;
            }
        }
    }
    return D;
}

void initGame(Game *game, int numPlayers)
{
    game->numPlayers = numPlayers;
    game->round = PREFLOP;
    game->pot = (20 * numPlayers);
    game->currentPlayer = 0;
    game->dealer = 0;
    game->smallBlind = 0;
    game->bigBlind = 0;
    game->numFolded = 0;
    game->numCalled = 0;
    game->numRaised = 0;
    game->numAllIn = 0;
    game->currentCall =0;

    for (int i = 0; i < numPlayers; i++)
    {
        game->players[i].chips = 980;
        game->players[i].bet = 0;
        game->players[i].raise = 0;
        game->players[i].move = CHECK;

    }

    makeDeck(&game->shuffleDeck);
    shuffleDeck(&game->shuffleDeck);
    dealCards(game);

}

void playerAction(Game *game, int playerIndex) 
{
    Player *player = &game->players[playerIndex];

    int action;

    printf("Player %d's turn. Chips: %d\n", playerIndex, player->chips);
    displayPlayerCards(player);
    if (game->round != PREFLOP)
    {
    displayCommunityCards(&game->communityCards);
    }
    // Display current game state
    printf("Current pot: %d\n", game->pot);
    printf("Current call: %d\n", game->currentCall);
    
    // Check if the player can check
    if (game->currentCall == 0) 
    {
        printf("Options: 1. Check  2. Bet  3. Fold\n");
    } else 
    {
        printf("Options: 1. Call  2. Raise  3. Fold\n");
    }
    
    // Get player action
    scanf("%d", &action);

    switch (action) 
    {
        case 1:
            if (game->currentCall == 0) 
            {
                player->move = CHECK;
                printf("Player %d checks.\n", playerIndex);
            } 
            else 
            {
                player->move = CALL;
                player->chips -= game->currentCall;
                player->bet += game->currentCall;
                game->pot += game->currentCall;
                printf("Player %d calls.\n", playerIndex);
            }
            break;

        case 2:
            if (game->currentCall == 0) 
            {
                // Place a bet
                int bet;
                printf("Enter bet amount: ");
                scanf("%d", &bet);
                player->chips -= bet;
                player->bet += bet;
                game->pot += bet;
                game->currentCall = bet;
                game->maxBet = bet;
                player->move = RAISE;
                printf("Player %d bets %d chips.\n", playerIndex, bet);
            } 
            else 
            {
                // Raise
                int raise;
                printf("Enter raise amount: ");
                scanf("%d", &raise);
                player->chips -= (game->currentCall + raise);
                player->bet += (game->currentCall + raise);
                game->pot += (game->currentCall + raise);
                game->currentCall += raise;
                game->maxBet += raise;
                player->move = RAISE;
                printf("Player %d raises by %d chips.\n", playerIndex, raise);
            }
            break;

        case 3:
            player->move = FOLD;
            printf("Player %d folds.\n", playerIndex);
            break;

        default:
            printf("Invalid action. Try again.\n");
            playerAction(game, playerIndex); // Recursively ask for a valid action
    }
    //char buffer[STRING_BUFFER];
    //snprintf("Player 1 Chips: %d, Player 2 Chips: %d, Player 3 Chips: %d, Player 4 Chips: %d", player[0]->chips, player[1]->chips, player[2]->chips, player[3]->chips);
}

/*
void playerAction(Game *game, int playerIndex) 
{
    Player *player = &game->players[playerIndex];
    int highestBet = findHighestBet(game);
    int action;

    printf("Player %d's turn. Chips: %d\n", playerIndex, player->chips);
    displayPlayerCards(player);
    displayCommunityCards(&game->communityCards);

    // Display current game state
    printf("Current pot: %d\n", game->pot);
    printf("Current call: %d\n", game->currentCall);
    
    if (game->currentCall == 0) 
    {
        printf("Options: 1. Check  2. Bet  3. Fold\n");
    } 
    else 
    {
        printf("Options: 1. Call  2. Raise  3. Fold\n");
    }
    
    // Get player action
    while (1) 
    {
        printf("Enter your action: ");
        if (scanf("%d", &action) != 1) 
        {
            // Invalid input, clear input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (action < 1 || action > 3) 
        {
            printf("Invalid action. Please enter a number between 1 and 3.\n");
            continue;
        }

        if ((action == 2 || action == 3) && game->currentCall == 0) 
        {
            printf("You can't raise or fold without a current call. Please choose again.\n");
            continue;
        }

        break; // Valid input, exit loop
    }

    switch (action) 
    {
        case 1:
            if (game->currentCall == 0) 
            {
                player->move = CHECK;
                printf("Player %d checks.\n", playerIndex);
            } 
            else 
            {
                player->move = CALL;
                player->chips -= game->currentCall;
                player->bet += game->currentCall;
                game->pot += game->currentCall;
                printf("Player %d calls.\n", playerIndex);
            }
            break;

        case 2:
            if (game->currentCall == 0) 
            {
                int bet;
                printf("Enter bet amount: ");
                while (1) 
                {
                    if (scanf("%d", &bet) != 1) 
                    {
                        // Invalid input, clear input buffer
                        while (getchar() != '\n');
                        printf("Invalid input. Please enter a number.\n");
                        continue;
                    }

                    if (bet <= 0) 
                    {
                        printf("Bet amount must be greater than 0. Please enter again.\n");
                        continue;
                    }

                    if (bet > player->chips) 
                    {
                        printf("You don't have enough chips to bet that amount. Please enter again.\n");
                        continue;
                    }

                    break; // Valid input, exit loop
                }
                player->chips -= bet;
                player->bet += bet;
                game->pot += bet;
                game->currentCall = bet;
                game->maxBet = bet;
                player->move = RAISE;
                printf("Player %d bets %d chips.\n", playerIndex, bet);
            } 
            else 
            {
                int raise;
                printf("Enter raise amount: ");
                while (1) 
                {
                    if (scanf("%d", &raise) != 1) 
                    {
                        // Invalid input, clear input buffer
                        while (getchar() != '\n');
                        printf("Invalid input. Please enter a number.\n");
                        continue;
                    }

                    if (raise <= 0) 
                    {
                        printf("Raise amount must be greater than 0. Please enter again.\n");
                        continue;
                    }

                    if (raise > player->chips || raise + game->currentCall > player->chips) 
                    {
                        printf("You don't have enough chips to raise that amount. Please enter again.\n");
                        continue;
                    }

                    break; // Valid input, exit loop
                }
                player->chips -= (game->currentCall + raise);
                player->bet += (game->currentCall + raise);
                game->pot += (game->currentCall + raise);
                game->currentCall += raise;
                game->maxBet += raise;
                player->move = RAISE;
                printf("Player %d raises by %d chips.\n", playerIndex, raise);
            }
            break;

        case 3:
            player->move = FOLD;
            printf("Player %d folds.\n", playerIndex);
            break;
    }
}
*/
void startGame(Game *game)
{
    for (int z = 1; z <= game->numPlayers; z++)
    {
        printf("Player %d's Chips: 1000\n", z);
    }

    printf("\nAnte is 20\n\n");

    for (int z = 1; z <= game->numPlayers; z++)
    {
        printf("Player %d's Chips: 980\n", z);
    }

    printf("\n");

    while(game->round <= RIVER)
    {
        for (int i = 0; i < game->numPlayers; i++)
        {
            if (game->players[i].move != FOLD)
            {
                playerAction(game, i);
            }
        }
        game->round++;

        if (game->round == FLOP)
        {
            game->communityCards.top = 3;
        }
        else if (game->round == TURN)
        {
            game->communityCards.top = 4;
        }
        else if (game->round == RIVER)
        {
            game->communityCards.top = 5;
        }
    }
}

void displayPlayerCards(Player *player)
{
    printf("Player Cards: %s of %s, %s of %s\n", ranks[player->card1.rank - 2], suits[player->card1.suit], ranks[player->card2.rank - 2], suits[player->card2.suit]);
}

void displayCommunityCards(Deck *communityCards) 
{
    printf("Community Cards: ");
    for (int i = 0; i < communityCards->top; i++)
    {
        printf("%s of %s ", ranks[communityCards->cards[i].rank - 2], suits[communityCards->cards[i].suit]);   
    }
    printf("\n");
}

int findHighestBet(Game *game)
{
    int highestBet = 0;
    for (int i = 0; i < game->numPlayers; i++)
    {
        if (game->players[i].bet > highestBet)
        {
            highestBet = game->players[i].bet;
        }
    }
    return highestBet;
}
