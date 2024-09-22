#include "Game.h"
#include "External.h"
#include <time.h>
#include "CrizeTris.h"

static int WIDTH;
static int HEIGHT;
static bool gameOver = false;
static int score = 0;
static char* map;



// Function to set the console font
void SetConsoleFont(const wchar_t* fontName, int fontSize) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontSize;
    cfi.dwFontSize.Y = fontSize;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}


void Setup(int width, int height){
    WIDTH = width;
    HEIGHT = height;
    
    system("cls");
    SetWindowSize(width,height+1);
    ShowsCursor(false);
    SetConsoleTitle("CrizeTris cmd");

    SetConsoleFont(L"Lucida Console", 16); //Consolas work too

    map = (char*)calloc(WIDTH * HEIGHT,sizeof(char));
    srand((unsigned)time(NULL));
    sPrint("Score: 0", 0, HEIGHT);
    
    // rand() % WIDTH;

    int board_pos[] = {5,7};
    int board_height = 20;
    int board_width = 10;
    int buffer = 20;
    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            if((i==board_pos[0]||i==board_pos[0]+board_width+1)&&j>board_pos[1]&&j<board_pos[1]+board_height+2){
                sColoredPrint("O", i, j, WHITE);
            }

            if(j==board_pos[1]+1+board_height && (i>board_pos[0] && i<board_pos[0]+board_width+1)){
                sColoredPrint("O", i, j, WHITE);
            }
        }
    }

    srand(time(NULL));
    CreateBoard(board_width, board_height, buffer);
    StartGame();
    SpawnPiece();
    int* piece_coords = GetPiecePos();

    PrintPiece(piece_coords,4);
    // int* new_pos = game_to_terminal_coords(piece_coords, 4);

    // cColoredPrint('#', new_pos[0], new_pos[1], YELLOW);
}

void PrintPiece(int* piece_coords, int count){
    int* new_pos = game_to_terminal_coords(piece_coords, count);
    for (size_t i = 0; i < count; i++)
    {
        cColoredPrint('#', new_pos[i*2], new_pos[i*2 + 1], YELLOW);
    }
}



void Play(){
    do {
        Move();
    } while(!gameOver);
    
    free(map);
    sPrint("You've lost!", HEIGHT/2, WIDTH/2-6);
}


void Move(){
    switch(getch()){
        case 77: //right key
            MovePiece(right);
        break;
        case 75: //left key
            MovePiece(left);
            break;
        case 72: //up key
            MovePiece(up);
            break;
        case 80: //down key
            MovePiece(down);
            break;
        case 27: //esc key
            gameOver = true;
            return;
    }
    int* pos = GetPiecePos();
    int* new_pos = game_to_terminal_coords(pos, 1);

    // printf("%d %d\n", new_pos[0], new_pos[1]);
   
    cColoredPrint('#', new_pos[0], new_pos[1], YELLOW);
}

int* game_to_terminal_coords(int* pos, int amount_of_pos){
    int* new_pos = malloc(amount_of_pos*2*sizeof(int));
    // int debug[] = {0,0};
    for (size_t i = 0; i < amount_of_pos; i++)
    {
        new_pos[i*2] = (pos[i*2]) + 5 + 1;
        new_pos[i*2 + 1] = (-pos[i*2 + 1]) + 7 + 20;
        // debug[0] = new_pos[i*2];
        // debug[1] = new_pos[i*2 + 1];
    }

    // new pos to array


    return new_pos;
}


void UpdateScore(){
    score++;
    char sscore[20];
    itoa(score,sscore, 10);
    sPrint(sscore, HEIGHT, 12);
}
