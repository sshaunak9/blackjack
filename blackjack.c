//
// Created by Shaunak Shah on 1/26/18.
//

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_CARDS 52
#define SOFT 0
#define HARD 1
const char suits[] = {'S', 'C', 'H', 'D'};
char faces[13] = {'A','2','3','4','5','6','7','8','9','X','J','Q','K'};
typedef int Deck[NUM_CARDS];
enum who {
    PLAYER, DEALER
} whosPlaying;

///////////DEBUG METHODS////////////////////
void printSuit(char suit) {
    switch(suit) {
        case 'S':
            printf("Spade\n");
            break;
        case 'C':
            printf("Club\n");
            break;
        case 'D':
            printf("Diamond\n");
            break;
        case 'H':
            printf("Heart\n");
            break;
    }
}

void printCard(char prepend[], int cardNum) {
    printf("%s %c of ", prepend, faces[cardNum%13]);
    printSuit(suits[cardNum/13]);
}
///////////END DEBUG METHODS////////////////


void initDeck(Deck deck) {
    int i;
    for(i = 0; i < NUM_CARDS; i++) {
        deck[i] = i;
    }
}

char getCard(char prepend[], int cardNum) {
    printCard(prepend, cardNum);
    return faces[cardNum%13];
}

void shuffleDeck(Deck deck) {
    int i = 0;
    srand(time(0));
    for(i = 0 ; i < NUM_CARDS; i++) {
        int k = rand() % NUM_CARDS;
        int tmp = deck[k];
        deck[k] = deck[i];
        deck[i] = tmp;
    }
}

char displayMessageAndGetInput(char message[]) {
    char input;
    printf("%s", message);
    scanf(" %c",&input);
    getchar();
    return input;
}

/*
 * ACE soft == 11
 * ACE hard == 1
 * X, Jack, Queen, King == 10
 */
void calculateHighLowTotal(char card, int total[2]) {
    switch(card) {
        case 'A':
            total[SOFT] += 11;
            total[HARD] += 1;
            break;
        case 'X':
        case 'J':
        case 'Q':
        case 'K':
            total[SOFT] += 10;
            total[HARD] += 10;
            break;
        default:
            total[SOFT] += card - '0';
            total[HARD] += card - '0';
    }
    return;
}

void dealCard(int whosPlaying, Deck deck, int *deckIndex, int total[2]) {
    char *prepend;
    if(whosPlaying == PLAYER) {
        prepend = "Player draws:";
    } else {
        prepend = "Dealer draws:";
    }
    char card = getCard(prepend,deck[*deckIndex]);
    calculateHighLowTotal(card, total);
    (*deckIndex)++;
}

/*
 * Print player's or dealer's total
 * if (soft hand total > 21) or (soft hand total == hard hand total) :
 *      use hard hand total
 * else:
 *      use soft hand total
 *
 * return total
 */
int printTotal(int whosPlaying, int total[]) {
    char *prepend;
    if (whosPlaying == PLAYER) {
        prepend = "Player";
    } else {
        prepend = "Dealer";
    }
    int t = 0;
    if(total[SOFT] > 21 || total[SOFT] == total[HARD]) {
        t = total[HARD];
    } else {
        t = total[SOFT];
    }
    printf("%s Total: %d\n", prepend, t);
    return t;
}

/*
 * Returns busted status based on soft and hard points
 */
int isBusted(int total[2]) {
    return !(total[SOFT] <= 21 || total[HARD] <=21);
}

/*
 * https://en.wikipedia.org/wiki/Blackjack
 * Players win by not busting and having a total higher than the dealer,
 * or not busting and having the dealer bust,
 * or getting a blackjack without the dealer getting a blackjack.
 */
void decideWinner(int finalScores[2], int finalWins[2]) {

    // player not bust && (playerscore > dealerscore || dealer bust)
    if (finalScores[PLAYER] <= 21 && (finalScores[PLAYER] > finalScores[DEALER] || finalScores[DEALER] > 21) ) {
        printf("Player wins !! :-)\n");
        (finalWins[PLAYER])++;
        return;
    }

    // player blackjack && dealer no blackjack
    if (finalScores[PLAYER] == 21 && (finalScores[DEALER] != 21)) {
        printf("Player wins !! :-)\n");
        (finalWins[PLAYER])++;
        return;
    }
    (finalWins[DEALER])++;
    printf("Dealer wins :-(\n");
}


///////TEST CASES/////
void testDecideWinner(){
    printf("\n--testDecideWinner--\n");

    int finalWins[2] = {0,0};

    //player not bust and player > dealer --> player wins
    int finalScores[2] = {21,20};
    decideWinner(finalScores,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
    //player not bust and dealer bust --> player wins
    int finalScores1[2] = {19,25};
    decideWinner(finalScores1,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
    //player blackjack dealer no blackjack > 21 --> player wins
    int finalScores2[2] = {21,22};
    decideWinner(finalScores2,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
    //player blackjack dealer no blackjack < 21 --> player wins
    int finalScores3[2] = {21,19};
    decideWinner(finalScores3,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
    //dealer blackjack player bust --> dealer wins
    int finalScores4[2] = {24,21};
    decideWinner(finalScores4,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
    //dealer not bust player not bust and dealer > player --> dealer wins
    int finalScores5[2] = {19,21};
    decideWinner(finalScores5,&finalWins);
    printf("Player wins: %d. Dealer wins: %d.\n", finalWins[PLAYER], finalWins[DEALER]);
}

void testPrintTotal() {
    printf("\n--testPrintTotal--\n");
    int a[2] = {5,15};//Ace and 4 (hard = 5, soft = 15)
    printTotal(PLAYER,a);
    int b[2] = {2,22};//Ace and Ace (hard = 2, soft = 22)
    printTotal(PLAYER,b);
    int c[2] = {15,22};//4, K, Ace (hard = 15, soft = 25)
    printTotal(DEALER,c);
    int d[2] = {22,32};//4, K, 7, Ace (hard = 22, soft = 32)
    printTotal(DEALER,d);
}

void testIsBusted() {
    printf("\n--testIsBusted--\n");
    int a[2] = {5,15};//Ace and 4 (hard = 5, soft = 15)
    printf("Busted = %d\n", isBusted(a));
    int b[2] = {2,22};//Ace and Ace (hard = 2, soft = 22)
    printf("Busted = %d\n", isBusted(b));
    int c[2] = {15,22};//4, K, Ace (hard = 15, soft = 25)
    printf("Busted = %d\n", isBusted(c));
    int d[2] = {22,32};//4, K, 7, Ace (hard = 22, soft = 32)
    printf("Busted = %d\n", isBusted(d));
}

///////END TEST CASES/////

int main() {
    char continuePlaying, hitOrStand;
    int gameCount = 0;
    Deck d;
    initDeck(d);
    int deckIndex = 0;
    int playerTotal[2], dealerTotal[2];
    int finalWins[2] = {0,0};
    int finalScores[2] = {0,0};
    int isPlayerBusted = 0, isDealerBusted = 0;
    float winPercentage = 0.0;
    do {
        printf("\n\n");
        //-------Init--------
        int i = 0;
        /*
         * Re-Initialize both player and dealer scores
         */
        for (i = 0; i < 2; i++) {
            playerTotal[i] = 0;dealerTotal[i] = 0;
        }
        /*
         * Shuffle Deck every 6 games
         */
        if((gameCount % 6) == 0) {
            printf("---------- GAME %d ---------- Shuffling cards. Have Fun ! ---------- \n\n", (gameCount + 1));
            shuffleDeck(d);
            deckIndex = 0;
        } else {
            printf("---------- GAME %d ---------- Have Fun ! ---------- \n\n", (gameCount + 1));
        }

        //-------Deal Initial cards--------
        dealCard(DEALER, d, &deckIndex, dealerTotal);
        printf("\n");
        dealCard(PLAYER, d, &deckIndex, playerTotal);
        dealCard(PLAYER, d, &deckIndex, playerTotal);
        printTotal(PLAYER, playerTotal);
        printf("\n");

        //-------Ask player's choice--------
        do {
            finalScores[PLAYER] = printTotal(PLAYER, playerTotal);
            if (finalScores[PLAYER] == 21) {
                //Break -- Player has 21. No need to hit or stand anymore.
                break;
            }
            
            hitOrStand = displayMessageAndGetInput("Hit/Stand ? H/S: ");
            if(hitOrStand == 'h' || hitOrStand == 'H') {
                dealCard(PLAYER, d, &deckIndex, playerTotal);
            }
            isPlayerBusted = isBusted(playerTotal);

            if (isPlayerBusted) {
                printf("Player Busted !! Try Again\n\n");
                break;
            }
        } while ((hitOrStand == 'h' || hitOrStand == 'H'));

        printf("\n");
        //-------Show dealers face down card--------
        dealCard(DEALER, d, &deckIndex, dealerTotal);

        printf("\n");
        //-------Dealers choice--------
        do {
            dealCard(DEALER, d, &deckIndex, dealerTotal);
            isDealerBusted = isBusted(dealerTotal);
            finalScores[DEALER] = printTotal(DEALER, dealerTotal);
            if (isDealerBusted) {
                printf("Dealer Busted !!\n\n");
                break;
            }
            if (finalScores[DEALER] == 21) {
                //Break -- Dealer has 21. No need to hit or stand anymore.
                break;
            }
        } while (dealerTotal[SOFT] <= 17);

        //-------Decide winner--------
        decideWinner(finalScores, &finalWins);

        gameCount++;
        winPercentage = (float)finalWins[PLAYER]/gameCount;
        printf("Player wins: %d. Dealer wins: %d. Player win percentage = %.2f\n", finalWins[PLAYER], finalWins[DEALER], winPercentage);
        if (winPercentage >= 0.5 && gameCount >= 3) {
            printf("--You have %.2f winning ratio. Consider walking away from the table to avoid losing everything--\n", winPercentage);
        }
        continuePlaying = displayMessageAndGetInput("\nContinue Playing ? Y/N: ");
    } while (continuePlaying == 'y' || continuePlaying == 'Y');
    winPercentage = (float)finalWins[PLAYER]/gameCount;
    printf("\n\n\n-----FINAL RESULT-----\n");
    printf("Player wins: %d. Dealer wins: %d. Player win percentage = %.2f\n", finalWins[PLAYER], finalWins[DEALER], winPercentage);

    /*printf("\n\n ----Testing----\n");
    testDecideWinner();
    testPrintTotal();
    testIsBusted();*/
    return 0;
}