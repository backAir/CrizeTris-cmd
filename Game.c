#include "Game.h"
#include "External.h"
#include <minwindef.h>
#include <stdint.h>
#include <time.h>
#include "CrizeTris.h"

static int WIDTH;
static int HEIGHT;
static bool gameOver = false;
static int score = 0;
static char* map;

int* last_piece_pos;

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

    SetConsoleOutputCP(CP_UTF8);
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
                sColoredPrint("█", i, j, WHITE);
            }

            if(j==board_pos[1]+1+board_height && (i>board_pos[0] && i<board_pos[0]+board_width+1)){
                sColoredPrint("█", i, j, WHITE);
            }
        }
    }

    srand(time(NULL));

    last_piece_pos = calloc(8,sizeof(int));
    CreateBoard(board_width, board_height, buffer);
    StartGame();
    SpawnPiece();
    int* piece_coords = GetCurrentPiecePos();
    PrintPiece(piece_coords,4,false);

    // int* new_pos = game_to_terminal_coords(piece_coords, 4);

    // cColoredPrint('#', new_pos[0], new_pos[1], YELLOW);
}



void PrintPiece(int* piece_coords, int count, bool placed_piece){
    
    int* new_pos = game_to_terminal_coords(piece_coords, count);
    if(!placed_piece){
        for (size_t i = 0; i < count; i++)
        {
            cColoredPrint(' ',last_piece_pos[i*2], last_piece_pos[i*2 + 1], YELLOW);
        }
    }
    
    for (size_t i = 0; i < count; i++)
    {
        sColoredPrint("█", new_pos[i*2], new_pos[i*2 + 1], YELLOW);
        last_piece_pos[i*2] = new_pos[i*2];
        last_piece_pos[i*2 + 1] = new_pos[i*2 + 1];
    }

}



void Play(){
    do {
        Sleep(1000/60);
        Move();
    } while(!gameOver);
    
    free(map);
    sPrint("You've lost!", HEIGHT/2, WIDTH/2-6);
}



typedef struct {
    int virtualKey;
    int moveFlag;
} KeyMapping;

KeyMapping keyMappings[] = {
    {VK_ESCAPE, 0}, // Special case for gameOver
    {VK_RIGHT, KEY_RIGHT},
    {VK_LEFT, KEY_LEFT},
    {VK_UP, KEY_UP},
    {VK_DOWN, KEY_DOWN},
    {VK_SPACE, KEY_HARDDROP},
    {'W', KEY_ROTATECCW},
    {'X', KEY_ROTATECW},
    {'C', KEY_HOLD},
    {'Q', KEY_180}
};

void Move(){
    int16_t input = 0;
    if(GetKeyState(VK_ESCAPE)&0x8000){
        gameOver = true;
        return;
    }
    for (int i = 0; i < sizeof(keyMappings) / sizeof(KeyMapping); i++) {
        if (GetKeyState(keyMappings[i].virtualKey) & 0x8000) {
            input |= keyMappings[i].moveFlag;
        }
    }

    bool piece_was_placed = GameLoop(input);
    int* piece_coords = GetCurrentPiecePos();
    PrintPiece(piece_coords,4,piece_was_placed);

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
