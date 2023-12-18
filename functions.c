#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wordhunt.h"

TrieNode trieNodes[612020];         //num nodes for dictionary required
int nodeIndex = 0;

TrieNode* initializeDictionary(void) {
    TrieNode* root = makeNode();

    FILE* dictFile = NULL;
    dictFile = fopen("dict.txt", "r");
    if (dictFile == NULL) {
        printf("Cannot open dictionary\n");
        return NULL;
    }

    char inputWord[16];
    while (fscanf(dictFile, "%s", inputWord) == 1) {
        insertWord(root, inputWord);
    }

    return root;
}

TrieNode* makeNode(void) {
    TrieNode* node = &trieNodes[nodeIndex++];

    for (int i = 0; i < N; ++i) {
        node->children[i] = NULL;
    }
    node->leaf = false;
    node->found = false;

    return node;
}

void insertWord(TrieNode* root, const char* word) {
    TrieNode* temp = root;

    for (int i = 0; word[i] != '\0'; ++i) {
        int position = (int)(word[i] - 'a');

        if (temp->children[position] == NULL) {             //Create node if not already
            temp->children[position] = makeNode();
        }

        temp = temp->children[position];                    //Go to next letter
    }
    temp->leaf = true;                                      //Mark as leaf
}

Board getBoard(void) {
    Board board;

    printf("Enter %d letters:\n", (SIZEBOARD * SIZEBOARD));

    for (int i = 0; i < SIZEBOARD; ++i) {
        char userChar = '\0';
        bool letterCheck = false;               /* Every new input, if a non-alphabetical character is inputted, */
                                                /* a warning is given and input is omitted */
        for (int j = 0; j < SIZEBOARD; ++j) {
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

void printBoard(const Board board) {
    printf("\nEntered board:\n\n");

    for (int i = 0; i < SIZEBOARD; ++i) {
        for (int j = 0; j < SIZEBOARD; ++j) {
            printf("%c ", board.tiles[i][j].letter);
        }
        printf("\n");
    }
}

void findWords(Board board, Word word, int row, int col, TrieNode* root, int* counter, Word* foundWords) {
    /* Check if tile is out of boundaries */
    if (row < 0 || row >= SIZEBOARD || col < 0 || col >= SIZEBOARD) {
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
        ++(*counter);
    }

    if (wordStatus == -1 || wordStatus == 1) {                              //Check children
        return;
    }

    findWords(board, word, row - 1, col, root, counter, foundWords);          //Up
    findWords(board, word, row + 1, col, root, counter, foundWords);          //Down
    findWords(board, word, row, col - 1, root, counter, foundWords);          //Left
    findWords(board, word, row, col + 1, root, counter, foundWords);          //Right
    findWords(board, word, row - 1, col - 1, root, counter, foundWords);      //Up left
    findWords(board, word, row - 1, col + 1, root, counter, foundWords);      //Up right
    findWords(board, word, row + 1, col - 1, root, counter, foundWords);      //Down left
    findWords(board, word, row + 1, col + 1, root, counter, foundWords);      //Down right

    return;
}

int searchWord(TrieNode* root, const char* word) {
    TrieNode* temp = root;

    for (int i = 0; word[i] != '\0'; ++i) {

        int position = (int)(word[i] - 'a');
        if (temp->children[position] == NULL) {                                     //If letter is not in tree, return -1
            return -1;                               
        }
        temp = temp->children[position];                                            //If yes, check next letter. Repeats until end of word
    }
    if (temp != NULL && temp->leaf == true && temp->found == false) {               //If at end of word, is leaf, not dupe, return true
        temp->found = true;                                                         //Mark duplicate
        for (int i = 0; i < N; ++i) {
            if (temp->children != NULL) {
                return 2;
            }
        }
        return 1;
    }
    
    return 0;                                                                       //Final case: formed, no children
}
//-1: word cannot be formed
// 0: word not leaf, but has children
// 1: word is leaf, no children
// 2: word is leaf, with children

int wordCompare(const void *wordA, const void *wordB) {
    Word *word1 = (Word *)wordA;
    Word *word2 = (Word *)wordB;

    return (word1->length - word2->length);
}
