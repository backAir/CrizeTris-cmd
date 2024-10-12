#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <stdlib.h>
#include "External.h"
#include <time.h>
#include "External.h"
#include <minwindef.h>
#include <stdint.h>
#include <time.h>

#include "CrizeTris/CrizeTris.h"



//array of pieces colors: 


void Setup(int width, int height);
void Play();
void Move();
void PrintPiece(int* piece_coords, int count, bool placed_piece);
void UpdateScore();
int* game_to_terminal_coords(int* pos, int amount_of_pos);
Pos game_to_terminal_coord(struct Pos pos);
void PrintBoard(int8_t* board, int board_width, int board_height, int board_buffer);
void PrintHold(int held_piece);
#endif
