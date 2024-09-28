#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <stdlib.h>
#include "External.h"
#include <time.h>




typedef struct POS{
    int y;
    int x;
}POS;

void Setup(int width, int height);
void Play();
void Move();
void PrintPiece(int* piece_coords, int count, bool placed_piece);
void UpdateScore();
int* game_to_terminal_coords(int* pos, int amount_of_pos);
POS game_to_terminal_coord(struct POS pos);
void PrintBoard(int8_t* board, int board_width, int board_height, int board_buffer);

#endif
