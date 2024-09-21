#include "CrizeTris.h"
#include <stdlib.h>

int8_t* board;
int board_width;
int board_height;

int current_bag[] = {[13] = 0};
int piece_position[] = {0,0};

void create_board(int width, int height, int buffer){
    board = calloc(width * (height+buffer), sizeof(int8_t));
    board_width = width;
    board_height = height;
}


void start_game(){
    piece_position[0] = 0;
    piece_position[1] = 0;
}

void move_piece(enum direction direction){
    switch (direction) {
        case up:
            piece_position[1] += 1;
            break;
        case down:
            piece_position[1] -= 1;
            break;
        case left:
            piece_position[0] -= 1;
            break;
        case right:
            piece_position[0] += 1;
            break;
    }

}

int* getPiecePos(){
    return piece_position;
};

void place_piece(int x, int y, int8_t piece, int rotation){
    switch (piece) {
        case O:
            board[x + y * board_width] = piece;
            board[(x + 1) + y * board_width] = piece;
            board[x + (y + 1) * board_width] = piece;
            board[(x + 1) + (y + 1) * board_width] = piece;
            break; 
    }
}
