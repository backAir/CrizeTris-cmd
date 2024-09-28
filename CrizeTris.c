#include "CrizeTris.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


int8_t* board;
int board_width;
int board_height;

int current_piece;
int current_bag[] = {[13] = 0};
int bag_position = 0;
int current_rotation = 0;
int piece_position[] = {0,0};
bool piece_is_placed = false;

void CreateBoard(int width, int height, int buffer){
    board = calloc(width * (height+buffer), sizeof(int8_t));
    board_width = width;
    board_height = height;
}

int timeSinceLastDrop = 0;
int lastInput = 0;
void StartGame(){ 
    timeSinceLastDrop = clock();
    piece_position[0] = 0;
    piece_position[1] = 0;
    GenerateBag(current_bag,0);
    GenerateBag(current_bag,7);
}


const int DROP_INTERVAL = 250; // 1/4th of a second in milliseconds
bool GameLoop(int16_t input){
    int pressedInput = input & ~lastInput;
    int heldInput = input & lastInput;
    bool piece_was_placed = false;





    if(pressedInput& KEY_ROTATECW){
        RotatePiece(clockwise);
    }
    if(pressedInput& KEY_ROTATECCW){
        RotatePiece(anticlockwise);
    }
    if(pressedInput & KEY_RIGHT){
        MovePiece(right);
    }
    if(pressedInput & KEY_LEFT){
        MovePiece(left);
    }
    if(pressedInput & KEY_UP){
        MovePiece(up);
    }

    int currentTime = clock();
    if (
        (input & KEY_DOWN) ||
        ((currentTime - timeSinceLastDrop) * 1000 / CLOCKS_PER_SEC >= DROP_INTERVAL)
    ) {
        piece_was_placed = MovePiece(down);
        timeSinceLastDrop = currentTime;
    }


    lastInput = input;
    return piece_was_placed;
}

void SpawnPiece(){
    piece_is_placed = false;
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




bool MovePiece(enum direction direction){
    int new_position[] = {piece_position[0],piece_position[1]};
    
    switch (direction) {
        case up:
            new_position[1] += 1;
            break;
        case down:
            new_position[1] -= 1;
            break;
        case left:
            new_position[0] -= 1;
            break;
        case right:
            new_position[0] += 1;
            break;
    }

    if(IsLegalPosition(new_position[0],new_position[1],current_piece,current_rotation, board, board_width)){
        piece_position[0] = new_position[0];
        piece_position[1] = new_position[1];
    }else if(direction == down){
        PlacePiece(piece_position[0],piece_position[1],current_piece,current_rotation);
        return true;
    }
    return false;
}

void RotatePiece(enum rotations rotation){
    int new_rotation = current_rotation;
    switch (rotation) {
        case clockwise:
            new_rotation++;
            if(new_rotation>3){
                new_rotation = 0;
            }
            break;
        case anticlockwise:
            new_rotation--;
            if(new_rotation<0){
                new_rotation = 3;
            }
            
            break;
    }
    bool legal = IsLegalPosition(piece_position[0],piece_position[1],current_piece,new_rotation, board, board_width);
    if(legal){
        current_rotation = new_rotation;
    }
}

int* GetCurrentPiecePos(){
    return GetPiecePos(piece_position[0],piece_position[1],current_rotation,current_piece);
}

void PlacePiece(int x, int y, int8_t piece, int rotation){
    int* piece_pos = GetPiecePos(x,y,rotation,piece);
    for (size_t i = 0; i < 4; i++)
    {
        board[piece_pos[i*2] + piece_pos[i*2 + 1] * board_width] = piece;
    }
    free(piece_pos);

    ClearLines(board);
    SpawnPiece();
    piece_is_placed = true;
}


int8_t* GetCurrBoard(){
    return board;
}
