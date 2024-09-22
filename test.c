#include "game.h"
#include "CrizeTris.h"
#include <stdio.h>

int test() {
    // Create a new game
    int board_pos[] = {5,7};
    int board_height = 20;
    int board_width = 10;
    int buffer = 20;

    CreateBoard(board_width, board_height, buffer);
    StartGame();
    SpawnPiece();
    int* piece_coords = GetPiecePos();
    printf("lol");
    // PrintPiece(piece_coords,4);
    return 0;
}