#include "CrizeTris.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>



int8_t* board;
//constants
int board_width;
int board_height;
int piece_spawn[] = {4,20};

//variables
int current_piece;
int current_bag[] = {[13] = 0};
int bag_position = 0;
int current_rotation = 0;
int held_piece = -1;
bool already_held = false;
int piece_position[] = {0,0};


void CreateBoard(int width, int height, int buffer){
    board = calloc(width * (height+buffer), sizeof(int8_t));
    board_width = width;
    board_height = height;
}

int timeSinceLastDrop = 0;
int lastInput = 0;
int ARR = 20;
int ARR_timer = 0;
int DAS = 113;
int DAS_timer = 0;
int Lock_down = 500;
int Lock_down_timer = 0;
void StartGame(){
    timeSinceLastDrop = DAS_timer = ARR_timer = Lock_down_timer = clock();
    piece_position[0] = 0;
    piece_position[1] = 0;
    GenerateBag(current_bag,0);
    GenerateBag(current_bag,7);
    SpawnPiece(true);
}

const int DROP_INTERVAL = 500;
int GameLoop(int16_t input){
    int pressedInput = input & ~lastInput;
    int heldInput = input & lastInput;
    int piece_was_placed = false;


    if(pressedInput& KEY_ROTATECW){
        RotatePiece(clockwise);
    }
    if(pressedInput& KEY_ROTATECCW){
        RotatePiece(anticlockwise);
    }
    if(pressedInput & KEY_RIGHT){
        MovePiece(right,false);
        DAS_timer = clock();
    }
    if(pressedInput & KEY_LEFT){
        MovePiece(left,false);
        DAS_timer = clock();
    }
    if(pressedInput & KEY_UP){
        RotatePiece(clockwise);
    }

    if(!already_held && (pressedInput & KEY_HOLD)){
        if(held_piece == -1){
            held_piece = current_piece;
            SpawnPiece(true);
        }else{
            int temp = current_piece;
            current_piece = held_piece;
            held_piece = temp;
            SpawnPiece(false);
        }
        already_held = true;
    }

    //todo: Maybe fix up ARR a bit ? idk
    if(heldInput & KEY_LEFT){
        if(((clock()-DAS_timer)*1000/CLOCKS_PER_SEC) >= DAS){
            if (((clock() - ARR_timer) * 1000 / CLOCKS_PER_SEC) >= ARR) {
                MovePiece(left,false);
                ARR_timer = clock();
            }
        }
    }
    if(heldInput & KEY_RIGHT){
        if(((clock()-DAS_timer)*1000/CLOCKS_PER_SEC) >= DAS){
            if (((clock() - ARR_timer) * 1000 / CLOCKS_PER_SEC) >= ARR) {
                MovePiece(right,false);
                ARR_timer = clock();
            }
        }
    }

    


    int currentTime = clock();
    if(pressedInput & KEY_HARDDROP){
        HardDrop();
        piece_was_placed = true;
        goto pieceplaced;
    }
    if ((input & KEY_DOWN) || (((currentTime - timeSinceLastDrop) * 1000 / CLOCKS_PER_SEC) >= DROP_INTERVAL)) {
        piece_was_placed = MovePiece(down,false);
        pieceplaced:
        timeSinceLastDrop = currentTime;
    }

    bool game_is_over = CheckGameOver();
    if(game_is_over){
        return -1;
    }
    lastInput = input;
    return piece_was_placed;
}

bool CheckGameOver(){
    return !IsLegalPosition(piece_position[0],piece_position[1],current_piece,current_rotation, board, board_width);
}


void SpawnPiece(bool new_piece){
    current_rotation = 0;
    piece_position[0] = piece_spawn[0];
    piece_position[1] = piece_spawn[1];
    if(!new_piece){return;}

    current_piece = current_bag[0];
    bag_position++;
    for (size_t i = 0; i < 13; i++)
    {
        current_bag[i] = current_bag[i+1];
    }
    if(bag_position==7){
        GenerateBag(current_bag,7);
        bag_position=0;
    }
    already_held = false;
}

void HardDrop(){
    //Makes the piece go down untill it's down all the way
    while(!MovePiece(down,true));
}


bool MovePiece(enum direction direction, bool hardDrop){
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
        Lock_down_timer = clock();
    }else if(direction == down){

        if (hardDrop||((clock() - Lock_down_timer) * 1000 / CLOCKS_PER_SEC) >= Lock_down) {
            PlacePiece(piece_position[0], piece_position[1], current_piece, current_rotation);
            Lock_down_timer = 0;
            return true;
        }
        return false;
    }
    return false;
}



void RotatePiece(enum rotations rotation){
    int new_rotation = current_rotation;
    int kick_table_index;

    switch (rotation) {
        case clockwise:
            new_rotation++;
            if(new_rotation>3){
                new_rotation = 0;
            }
            kick_table_index = current_rotation;
            break;
        case anticlockwise:
            new_rotation--;
            if(new_rotation<0){
                new_rotation = 3;
            }
            kick_table_index = current_rotation-1;  
            if(kick_table_index < 0){kick_table_index = 3;}
            break;
    }
    bool legal = IsLegalPosition(piece_position[0],piece_position[1],current_piece,new_rotation, board, board_width);
    if(legal){
        current_rotation = new_rotation;
    }else
    {
        char *table;
        
        if(current_piece == I){
            table = (char*) I_kick_table[kick_table_index];
        }else
        {
            table = (char*) kick_table[kick_table_index];
        }
        for (size_t i = 0; i < 4; i++)
        {
            //if rotating anticlockwise then multiply all table values by -1
            int flipTable = (rotation == anticlockwise) ? -1 : 1;
            int new_x = table[i*2]*flipTable+piece_position[0];
            int new_y = table[i*2+1]*flipTable+piece_position[1];
            bool is_legal = IsLegalPosition(new_x,new_y,current_piece,new_rotation, board, board_width);
            if(is_legal){
                piece_position[0] = new_x;
                piece_position[1] = new_y;
                current_rotation = new_rotation;
                return;
            }
        }
    }
}

int* GetCurrentPiecePos(){
    return GetPiecePos(piece_position[0],piece_position[1],current_rotation,current_piece);
}
int GetPiece(){
    return current_piece;
}

void PlacePiece(int x, int y, int8_t piece, int rotation){
    int* piece_pos = GetPiecePos(x,y,rotation,piece);
    for (size_t i = 0; i < 4; i++)
    {
        board[piece_pos[i*2] + piece_pos[i*2 + 1] * board_width] = piece;
    }
    free(piece_pos);

    ClearLines(board);
    SpawnPiece(true);
}


int8_t* GetCurrBoard(){
    return board;
}
int GetCurrHold(){
    return held_piece;
}
int* GetCurrBag(){
    return current_bag;
}