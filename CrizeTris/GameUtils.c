#include "GameUtils.h"


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

PieceInfo getPieceInfo(int piece) {
    PieceInfo info;
     const int8_t* piece_array;
    switch (piece)
    {
        case I:
            info.size = I_SIZE;
            piece_array = (int8_t*) I_PIECE;
        break;
        case O:
            info.size = O_SIZE;
            piece_array = (int8_t*) O_PIECE;
        break;
        case T:
            info.size = T_SIZE;
            piece_array = (int8_t*) T_PIECE;
        break;
        case S:
            info.size = S_SIZE;
            piece_array = (int8_t*) S_PIECE;
        break;
        case Z:
            info.size = Z_SIZE;
            piece_array = (int8_t*) Z_PIECE;
        break;
        case J:
            info.size = J_SIZE;
            piece_array = (int8_t*) J_PIECE;
        break;
        case L:
            info.size = L_SIZE;
            piece_array = (int8_t*) L_PIECE;
        break;
        default:
            printf("Error: Invalid piece type\n");
            exit(EXIT_FAILURE);
        break;
    }
    info.piece_array = piece_array;
    return info;
}


int* GetPiecePos(int x, int y, int rotation, int piece){

    int* piece_pos = malloc(8*sizeof(int)); //4*2 array of piece positions
   

    PieceInfo piece_info = getPieceInfo(piece);
    int size = piece_info.size;
    
    const int8_t* piece_array = piece_info.piece_array;


    int pice_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (piece_array[(rotation*size*size)+i+j*size]==1)
            {
                piece_pos[pice_count*2] = x + i;
                piece_pos[pice_count*2 + 1] = y+ size - j - 1;
                pice_count++;
            }
        }
    }
    #ifdef DEBUG
        int debug[4][2];
        for (size_t i = 0; i < 4; i++)
        {
            debug[i][0] = piece_pos[i*2];
            debug[i][1] = piece_pos[i*2 + 1];
        }
        (void) debug;
   #endif

    return piece_pos;
};


bool IsLegalPosition(int x, int y, int8_t piece, int rotation, int8_t* board, int board_width){
    int* piece_pos = GetPiecePos(x,y,rotation,piece);
    for (size_t i = 0; i < 4; i++)
    {
        if(piece_pos[i*2] < 0 || piece_pos[i*2] >= board_width || piece_pos[i*2 + 1] < 0){
            return false;
        }
        if(board[piece_pos[i*2] + piece_pos[i*2 + 1] * board_width] != 0){
            return false;
        }
    }
    return true;
}


int ClearLines(int8_t* board){

    int board_width = 10;
    int board_height = 40;
    int lines_cleared = 0;

    for (size_t i = 0; i < board_height; i++)
    {
        bool line_full = true;
        for (size_t j = 0; j < board_width; j++)
        {
            if(board[j+i*board_width] == 0){
                line_full = false;
                break;
            }
        }
        if(line_full){
            lines_cleared++;
            for (size_t j = i; j < board_height-1; j++)
            {
                for (size_t k = 0; k < board_width; k++)
                {
                    board[k+j*board_width] = board[k+(j+1)*board_width];
                }
            }
            for (size_t j = 0; j < board_width; j++)
            {
                board[j+(board_height-1)*board_width] = 0;
            }
            i--;
        }
    }
    return lines_cleared;
}