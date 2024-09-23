#include "CrizeTris.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int8_t* board;
int board_width;
int board_height;

int current_piece;
int current_bag[] = {[13] = 0};
int bag_position = 0;
int current_rotation = 0;
int piece_position[] = {0,0};

void CreateBoard(int width, int height, int buffer){
    board = calloc(width * (height+buffer), sizeof(int8_t));
    board_width = width;
    board_height = height;
}

void StartGame(){ 
    piece_position[0] = 0;
    piece_position[1] = 0;
    GenerateBag(current_bag,0);
    GenerateBag(current_bag,7);
}

void SpawnPiece(){
    current_piece = current_bag[0];
    current_rotation = 0;
    piece_position[0] = 4;
    piece_position[1] = 20;
    bag_position++;
    for (size_t i = 0; i < 13; i++)
    {
        current_bag[i] = current_bag[i+1];
    }
    if(bag_position==7){
        GenerateBag(current_bag,7);
        bag_position=0;
    }
}

void GenerateBag(int* bag, int starting_pos){
    int new_bag[] = {1,2,3,4,5,6,7};

    for (int i = 0; i < 7; i++)
    {
        int rnd = rand() % (7-i);
        bag[i+starting_pos] = new_bag[rnd];
        for (int j = rnd; j < 6; j++)
        {
            new_bag[j] = new_bag[j+1]; 
        }
    }
    for (size_t i = 0; i < 7; i++)
    {
        new_bag[i] = bag[i];
    }
    (void)new_bag;    
}

void MovePiece(enum direction direction){
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

int* GetPiecePos(){

    int* piece_pos = malloc(8*sizeof(int)); //4*2 array of piece positions
    int size;
    const int8_t* piece;
    switch (current_piece)
    {
        case I:
            size = I_SIZE;
            piece = (int8_t*) I_PIECE;
        break;
        case O:
            size = O_SIZE;
            piece = (int8_t*) O_PIECE;
        break;
        case T:
            size = T_SIZE;
            piece = (int8_t*) T_PIECE;
        break;
        case S:
            size = S_SIZE;
            piece = (int8_t*) S_PIECE;
        break;
        case Z:
            size = Z_SIZE;
            piece = (int8_t*) Z_PIECE;
        break;
        case J:
            size = J_SIZE;
            piece = (int8_t*) J_PIECE;
        break;
        case L:
            size = L_SIZE;
            piece = (int8_t*) L_PIECE;
        break;
        default:
            printf("Error: Invalid piece type\n");
            exit(EXIT_FAILURE);
        break;
    }


    int pice_count = 0;
    for (size_t x = 0; x < size; x++)
    {
        for (size_t y = 0; y < size; y++)
        {
            if (piece[(current_rotation*size*size)+x+y*size]==1)
            {
                piece_pos[pice_count*2] = piece_position[0] + x;
                piece_pos[pice_count*2 + 1] = piece_position[1] - y;
                pice_count++;
            }
        }
    }
    int debug[4][2];
    for (size_t i = 0; i < 4; i++)
    {
        debug[i][0] = piece_pos[i*2];
        debug[i][1] = piece_pos[i*2 + 1];
    }
    (void) debug;
   
    return piece_pos;
};

void PlacePiece(int x, int y, int8_t piece, int rotation){
    switch (piece) {
        case O:
            board[x + y * board_width] = piece;
            board[(x + 1) + y * board_width] = piece;
            board[x + (y + 1) * board_width] = piece;
            board[(x + 1) + (y + 1) * board_width] = piece;
            break; 
    }
}
