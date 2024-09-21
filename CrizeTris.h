#include <stdint.h>

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

enum direction{
    up,
    down,
    left,
    right
};


void create_board(int width, int height, int buffer);
void place_piece(int x, int y, int8_t piece, int rotation);
void start_game();
void move_piece(enum direction direction);
int* getPiecePos();

