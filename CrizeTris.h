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

#define KEY_RIGHT 0x0001
#define KEY_LEFT 0x0002
#define KEY_UP 0x0004
#define KEY_DOWN 0x0008
#define KEY_HARDDROP 0x0010
#define KEY_ROTATECW 0x0020
#define KEY_ROTATECCW 0x0040
#define KEY_HOLD 0x0080
#define KEY_180 0x0100

void CreateBoard(int width, int height, int buffer);
void PlacePiece(int x, int y, int8_t piece, int rotation);
void SpawnPiece();
void StartGame();
void MovePiece(enum direction direction);
void GenerateBag(int* bag, int starting_pos);
int* GetPiecePos();
void GameLoop(int16_t input);

