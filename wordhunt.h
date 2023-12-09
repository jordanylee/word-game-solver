#define SIZE 4
#define N 26

typedef struct GameTile {
    char letter;
    bool used;
} Tile;

typedef struct GameBoard {
    Tile tiles[4][4];
} Board;

typedef struct CreatedWord {
    char input[17];
    int length;
} Word;

typedef struct TrieNode TrieNode;

void wordFind(Board board, Word word, int row, int col, TrieNode* root, int* counter, Word* foundWords);

void printBoard(Board board);

Board getBoard(void);

TrieNode* makeNode(char letter);

void freeTrie(TrieNode* node);

TrieNode* insertWord(TrieNode* root, char* word);

int searchWord(TrieNode* root, char* word);

void printTrie(TrieNode* root);

void printSearch(TrieNode* root, char* word);

TrieNode* initDict(void);

int wordCompare(const void *wordA, const void *wordB);
