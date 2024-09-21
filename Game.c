#include "Game.h"
#include "External.h"
#include <time.h>
#include "CrizeTris.h"

static int WIDTH;
static int HEIGHT;
static Entity player = { YELLOW, '@', 0, 0 };
static const Entity coin = { YELLOW, '$', 0, 0 };
static const Entity bomb = { DARKRED, 'o', 0, 0 };
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
    int i;
    
    // for(i = 0; i < amountOfBombs; i++){
    //     do{
    //         rndp.x = rand() % HEIGHT;
    //         rndp.y = rand() % WIDTH;
    //     }while(map[rndp.x * WIDTH + rndp.y] != 0);
    
    //     map[rndp.x * WIDTH + rndp.y] = bomb.c;
    //     cColoredPrint(bomb.c, rndp.x, rndp.y, bomb.color);
    // }

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

    create_board(board_width, board_height, buffer);


    
    // for(i = 0; i < amountOfCoins; i++){
    //     do{
    //         rndp.x = rand() % HEIGHT;
    //         rndp.y = rand() % WIDTH;
    //     }while(map[rndp.x * WIDTH + rndp.y] != 0);
    
    //     map[rndp.x * WIDTH + rndp.y] = coin.c;
    //     cColoredPrint(coin.c, rndp.x, rndp.y, coin.color);
    // }
    
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
            move_piece(right);
        break;
        case 75: //left key
            move_piece(left);
            break;
        case 72: //up key
            move_piece(up);
            break;
        case 80: //down key
            move_piece(down);
            break;
        case 27: //esc key
            gameOver = true;
            return;
    }
    int* pos = getPiecePos();
    int* new_pos = game_to_terminal_coords(pos, 1);

    cColoredPrint('#', new_pos[0], new_pos[1], YELLOW);
}

int* game_to_terminal_coords(int* pos, int amount_of_pos){
    int* new_pos = (int*)calloc(amount_of_pos*2, sizeof(int));
    new_pos[0] = pos[0] + 5 + 1;
    
    new_pos[1] = -pos[1] + 7 + 20;
    
    return new_pos;
}


void UpdateScore(){
    score++;
    char sscore[20];
    itoa(score,sscore, 10);
    sPrint(sscore, HEIGHT, 12);
}
