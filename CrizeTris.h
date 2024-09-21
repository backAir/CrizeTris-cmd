#include <stdint.h>
#include "Pieces.h"

enum pieces {
    empty = 0,
    I = 1,
    O = 2,
    T = 3,
    S = 4,
    Z = 5,
    J = 6,
    L = 7
};

enum piece_sizes {
    I_SIZE = 4,
    O_SIZE = 2,
    T_SIZE = 3,
    S_SIZE = 3,
    Z_SIZE = 3,
    J_SIZE = 3,
    L_SIZE = 3
};

enum direction{
    up,
    down,
    left,
    right
};


void CreateBoard(int width, int height, int buffer);
void PlacePiece(int x, int y, int8_t piece, int rotation);
void SpawnPiece();
void StartGame();
void MovePiece(enum direction direction);
void GenerateBag(int* bag, int starting_pos);
int* GetPiecePos();

