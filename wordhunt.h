extern int SIZEBOARD;
#define N 26

typedef struct CreatedWord {
    char input[17];
    int length;
} Word;

typedef struct GameTile {
    char letter;
    bool used;
} Tile;

typedef struct GameBoard {
    Tile tiles[4][4];
} Board;

typedef struct TrieNode {
    struct TrieNode* children[N];
    bool leaf;
    bool found;
} TrieNode;

TrieNode* initializeDictionary(void);
TrieNode* makeNode(void);
void insertWord(TrieNode* root, const char* word);
Board getBoard(void);
void printBoard(const Board board);
void findWords(Board board, Word word, int row, int col, TrieNode* root, int* counter, Word* foundWords);
int searchWord(TrieNode* root, const char* word);
int wordCompare(const void *wordA, const void *wordB);
