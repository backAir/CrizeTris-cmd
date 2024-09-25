#include <stdint.h>
#include "Pieces.h"
#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>


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

enum rotations{
    clockwise,
    anticlockwise
};

typedef struct {
    int size;
    const int8_t* piece_array;
} PieceInfo;


void GenerateBag(int* bag, int starting_pos);
PieceInfo getPieceInfo(int piece);
int* GetPiecePos(int x, int y, int rotation, int piece);
int* GetCurrentPiecePos();
bool IsLegalPosition(int x, int y, int8_t piece, int rotation, int8_t* board, int board_width);