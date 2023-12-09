#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordhunt.h"

typedef struct TrieNode {
    char letter;
    bool leaf;
    TrieNode* children[N];
    bool found;
} TrieNode;

TrieNode* makeNode(char letter) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));

    for (int i = 0; i < N; i++) {
        node->children[i] = NULL;
    }
    node->letter = letter;
    node->leaf = false;
    node->found = false;

    return node;
}

void freeTrie(TrieNode* node) {
    for (int i = 0; i < N; i++) {

        if (node->children[i] != NULL) {
            freeTrie(node->children[i]);
        }
    }
    free(node);
}

TrieNode* insertWord(TrieNode* root, char* word) {
    TrieNode* temp = root;

    for (int i = 0; word[i] != '\0'; i++) {

        int position = (int)(word[i] - 'a');
        if (temp->children[position] == NULL) {             //Create node if not already
            temp->children[position] = makeNode(word[i]);
        }

        temp = temp->children[position];                    //Go to next letter
    }
    temp->leaf = true;                                      //Mark as leaf

    return root;
}

int searchWord(TrieNode* root, char* word) {
    TrieNode* temp = root;

    for (int i = 0; word[i] != '\0'; i++) {

        int position = (int)(word[i] - 'a');
        if (temp->children[position] == NULL) {                                     //If letter is not in tree, return -1
            return -1;                               
        }
        temp = temp->children[position];                                            //If yes, check next letter. Repeats until end of word
    }
    if (temp != NULL && temp->leaf == true && temp->found == false) {               //If at end of word, is leaf, not dupe, return true
        temp->found = true;                                                         //Mark duplicate
        for (int i = 0; i < N; i++) {
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

TrieNode* initDict(void) {
    TrieNode* root = makeNode('\0');

    FILE* dictFile = NULL;
    dictFile = fopen("dict.txt", "r");
    if (dictFile == NULL) {
        printf("Cannot open dictionary\n");
        return NULL;
    }

    char inputWord[16];
    while (fscanf(dictFile, "%s", inputWord) == 1) {
        root = insertWord(root, inputWord);
    }

    return root;
}
