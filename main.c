#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "wordhunt.h"

int main(void) {
/*
    printf("Size of board?\n");
    scanf("%d", &rows);
    cols = rows;
    */
    TrieNode* root = initDict();

    Word foundWords[1250];
    for (int i = 0; i < 1250; i++) {
        foundWords[i].input[0] = '\0';
    }

    Word word;
    word.input[0] = '\0';
    int wordCounter = 0;

    Board board = getBoard();

    printBoard(board);

    /* starting timer */
    LARGE_INTEGER frequency;                            /* ticks per second */
    LARGE_INTEGER t1, t2;                               /* ticks */
    double elapsedTime;
    QueryPerformanceFrequency(&frequency);              /* get ticks per second */
    QueryPerformanceCounter(&t1);                       /* start timer */

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {
            wordFind(board, word, i, j, root, &wordCounter, foundWords);
        }
    }

    qsort(foundWords, wordCounter, sizeof(Word), wordCompare);

    QueryPerformanceCounter(&t2);
    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;    /* compute elapsed time in milliseconds */

    for (int i = 0; i < wordCounter; i++) {

        printf("%s\n", foundWords[i].input);
    }

    printBoard(board);
    
    printf("\nFound %d words\n", wordCounter);
    printf("%f ms elapsed time\n", elapsedTime);
    
    freeTrie(root);

    return 0;
}
