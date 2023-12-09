#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wordhunt.h"

Board getBoard(void) {
    Board board;

    printf("Enter %d letters:\n", (SIZE * SIZE));

    for (int i = 0; i < SIZE; i++) {
        
        char userChar = '\0';
        bool letterCheck = false;               /* Every new input, if a non-alphabetical character is inputted, */
                                                /* a warning is given and input is omitted */
        for (int j = 0; j < SIZE; j++) {

            do {
                userChar = getchar();

                if (!isalpha(userChar) && (!isspace(userChar)) && (letterCheck == false)) {
                    printf("Non-letters omitted.\n");
                    letterCheck = true;
                }

                if (isspace(userChar)) {
                    letterCheck = false;
                }
            }
            while (isspace(userChar) || !isalpha(userChar));
            
            board.tiles[i][j].letter = tolower(userChar);
            board.tiles[i][j].used = false;         /* Ensure all tiles begin unchecked */
        }
    }
    
    return board;
}

void printBoard(Board board) {
    printf("\nEntered board:\n\n");

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {

            printf("%c ", board.tiles[i][j].letter);
        }
        printf("\n");
    }
}

int wordCompare(const void *wordA, const void *wordB) {
    Word *word1 = (Word *)wordA;
    Word *word2 = (Word *)wordB;

    return (word1->length - word2->length);
}


void wordFind(Board board, Word word, int row, int col, TrieNode* root, int* counter, Word* foundWords) {
    /* Check if tile is out of boundaries */
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        /* printf("out of bounds\n"); */
        return;
    }

    /* Check if tile has been used */
    if (board.tiles[row][col].used) {
        //printf("visited\n");
        return;
    }

    //printf("Checking %d, %d (%c)\n", row, col, board.tiles[row][col].letter);
    
    board.tiles[row][col].used = true;
    strncat(word.input, &board.tiles[row][col].letter, 1);
    word.length = strlen(word.input);
    int wordStatus = -1;
    wordStatus = searchWord(root, word.input);

    if (word.length >= 3 && wordStatus >= 1) {
        strcpy(foundWords[*counter].input, word.input);
        foundWords[*counter].length = word.length;
        (*counter)++;
    }

    if (wordStatus == -1 || wordStatus == 1) {                              //Check children
        return;
    }

    wordFind(board, word, row - 1, col, root, counter, foundWords);          //Up
    wordFind(board, word, row + 1, col, root, counter, foundWords);          //Down
    wordFind(board, word, row, col - 1, root, counter, foundWords);          //Left
    wordFind(board, word, row, col + 1, root, counter, foundWords);          //Right
    wordFind(board, word, row - 1, col - 1, root, counter, foundWords);      //Up left
    wordFind(board, word, row - 1, col + 1, root, counter, foundWords);      //Up right
    wordFind(board, word, row + 1, col - 1, root, counter, foundWords);      //Down left
    wordFind(board, word, row + 1, col + 1, root, counter, foundWords);      //Down right

    return;
}
