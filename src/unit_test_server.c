/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* unit_test_server.c: Unit test for server			                 */
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_MESSAGE_LEN 255
#define PORT_NUMBER_LOWER_BOUND 1024
#define MAX_CARDS_PER_CLIENT 52

int getPortNum(int argc, char *const *argv);
int strlen_without_null(const char *str);
char** initializeCards();
void shuffleCards(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards);
int getFirstCardsIndex(char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1], int numCards);

void test_getPortNum_valid_port() {
    char *args[] = {"program", "1234"};
    int portNum = getPortNum(2, args);
    assert(portNum == 1234);
    printf("test_getPortNum_valid_port passed.\n");
}

void test_getPortNum_invalid_port() {
    char *args[] = {"program", "1023"};
    if (fork() == 0) {
        getPortNum(2, args);
        assert(0); // If exit is not called, test failed
    } else {
        int status;
        wait(&status);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);
        printf("test_getPortNum_invalid_port passed.\n");
    }
}

void test_strlen_without_null() {
    assert(strlen_without_null("hello\n") == 5);
    assert(strlen_without_null("hello") == 5);
    assert(strlen_without_null("") == 0);
    printf("test_strlen_without_null passed.\n");
}

void test_initializeCards() {
    char **cards = initializeCards();
    assert(cards != NULL);
    /*
    assert(strcmp(cards[0], "ace_hearts") == 0);
    assert(strcmp(cards[51], "king_spades") == 0);
    */
    for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
        free(cards[i]);
    }
    free(cards);
    printf("test_initializeCards passed.\n");
}

void test_shuffleCards() {
    char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1];
    for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
        snprintf(cards[i], sizeof(cards[i]), "card_%d", i);
    }
    shuffleCards(cards, MAX_CARDS_PER_CLIENT);
    // Just check that all cards are still present after shuffling
    int found[MAX_CARDS_PER_CLIENT] = {0};
    for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
        int cardNum;
        sscanf(cards[i], "card_%d", &cardNum);
        assert(cardNum >= 0 && cardNum < MAX_CARDS_PER_CLIENT);
        found[cardNum] = 1;
    }
    for (int i = 0; i < MAX_CARDS_PER_CLIENT; ++i) {
        assert(found[i]);
    }
    printf("test_shuffleCards passed.\n");
}

void test_getFirstCardsIndex() {
    char cards[MAX_CARDS_PER_CLIENT][MAX_MESSAGE_LEN + 1];
    for (int i = 0; i < MAX_CARDS_PER_CLIENT - 1; ++i) {
        strcpy(cards[i], "SENT");
    }
    strcpy(cards[MAX_CARDS_PER_CLIENT - 1], "card_52");
    assert(getFirstCardsIndex(cards, MAX_CARDS_PER_CLIENT) == MAX_CARDS_PER_CLIENT - 1);
    strcpy(cards[MAX_CARDS_PER_CLIENT - 1], "SENT");
    assert(getFirstCardsIndex(cards, MAX_CARDS_PER_CLIENT) == -1);
    printf("test_getFirstCardsIndex passed.\n");
}

int main() {
    test_getPortNum_valid_port();
    test_getPortNum_invalid_port();
    test_strlen_without_null();
    test_initializeCards();
    test_shuffleCards();
    test_getFirstCardsIndex();

    printf("All tests passed.\n");
    return 0;
}
