#include "Game.h"


static int WIDTH;
static int HEIGHT;
static bool gameOver = false;
static int score = 0;
static char* map;
int board_pos[] = {5,10};


int* last_piece_pos;


int8_t* new_board;
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
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleFont(L"Lucida Console", 16); //Consolas work too

    map = (char*)calloc(WIDTH * HEIGHT,sizeof(char));
    srand((unsigned)time(NULL));
    sPrint("Score: 0", 0, HEIGHT);
    
        // rand() % WIDTH;
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
    new_board = malloc(10*40*sizeof(int8_t));

    last_piece_pos = calloc(8,sizeof(int));
    CreateBoard(board_width, board_height, buffer);
    StartGame();
    SpawnPiece();
    int* piece_coords = GetCurrentPiecePos();
    PrintPiece(piece_coords,4,false);

    // int* new_pos = game_to_terminal_coords(piece_coords, 4);

    // cColoredPrint('#', new_pos[0], new_pos[1], YELLOW);
}



// void DisableEchoInput() {
//     HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
//     DWORD mode;
//     GetConsoleMode(hStdin, &mode);
//     mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
//     SetConsoleMode(hStdin, mode);
// }

void ClearInputBuffer() {
    while (_kbhit()) {
        ; // Read and discard character
        _getch();

    }
}


void Play(){
    do {
        ClearInputBuffer();
        clock_t start_time = clock();
        Move();
        clock_t end_time = clock();
        double runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000; // Convert to milliseconds
        double sleep_time = (1000.0 / 60.0) - runtime;
        if (sleep_time > 0) {
            Sleep((DWORD)sleep_time);
        } 

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

    int game_loop_return = GameLoop(input);
    if(game_loop_return==-1){
        gameOver = true;
    }
    int* piece_coords = GetCurrentPiecePos();
    // if(!piece_was_placed){
    //     // PrintPiece(piece_coords,4,piece_was_placed);
    // }
    int8_t* board = GetCurrBoard();

    memcpy(new_board, board, 10*40*sizeof(int8_t));
    for (size_t i = 0; i < 4; i++)
    {
        new_board[piece_coords[i*2] + piece_coords[i*2 + 1] * 10] = 1;
    }
    PrintBoard(new_board, 10, 20, 20);
}


int8_t* old_board = NULL;
void PrintBoard(int8_t* board, int board_width, int board_height, int board_buffer){
    if (old_board == NULL) {
        old_board = (int8_t*)calloc(board_width * (board_buffer+board_height), sizeof(int8_t));
        // memcpy(old_board, board, board_width * board_height * sizeof(int8_t));
    }
    
    for (size_t i = 0; i < board_width; i++)
    {
        for (size_t j = 0; j < board_height+board_buffer; j++)
        {
            int index = i + j * board_width;
            if (board[index] != old_board[index]) {
                // POS pos = {i,j};
                // pos = game_to_terminal_coord(pos);
                int pos[] = {i,j};
                int* new_pos = game_to_terminal_coords(pos, 1); 
                if(board[index] != 0){
                    sColoredPrint("█", new_pos[0], new_pos[1], YELLOW);
                }else{
                    sColoredPrint(" ", new_pos[0], new_pos[1], YELLOW);
                }
                free(new_pos);
            }
        }
    }
    memcpy(old_board, board, board_width * (board_buffer+board_height) * sizeof(int8_t));

}




bool last_was_placed = false;
void PrintPiece(int* piece_coords, int count, bool placed_piece){
    int* new_pos = game_to_terminal_coords(piece_coords, count);
    if(!last_was_placed){
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
    free(new_pos);
    last_was_placed = placed_piece;
}

struct Pos game_to_terminal_coord(struct Pos pos){
    struct Pos new_pos = {pos.x + board_pos[0] + 1, (-pos.y) + board_pos[1] + 20};
    return new_pos;
}


int* game_to_terminal_coords(int* pos, int amount_of_pos){
    int* new_pos = malloc(amount_of_pos*2*sizeof(int));
    // int debug[] = {0,0};
    for (size_t i = 0; i < amount_of_pos; i++)
    {
        new_pos[i*2] = (pos[i*2]) + board_pos[0] + 1;
        new_pos[i*2 + 1] = (-pos[i*2 + 1]) + board_pos[1] + 20;
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
