#include "Game.h"
#include <stdlib.h>

static int WIDTH;
static int HEIGHT;
static bool gameOver = false;
static int score = 0;
static char* map;

const int pieces_colors[] = {BLACK, CYAN, MAGENTA, YELLOW, GREEN, RED, BLUE, ORANGE};

int board_pos[] = {6,10};

int* last_piece_pos;

int8_t* new_board;
// Function to set the console font
void SetConsoleFont(const wchar_t* fontName, int fontSizeX, int fontSizeY) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontSizeX; // Set the width of the font
    cfi.dwFontSize.Y = fontSizeY; // Set the height of the font
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

int game_board_height = 20;
int game_board_width = 10;
int game_buffer = 20;

void Setup(int width, int height){
    WIDTH = width;
    HEIGHT = height;

    old_next = (int*)calloc(4 * 4, sizeof(int));

    
    system("cls");
    SetWindowSize(width,height+1);
    ShowsCursor(false);
    SetConsoleTitle("QualityTris cmd");

    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleFont(L"Lucida Console", 16, 16); //Consolas work too

    map = (char*)calloc(WIDTH * HEIGHT,sizeof(char));
    srand((unsigned)time(NULL));
    sPrint("Score: 0", 0, HEIGHT);
    
        // rand() % WIDTH;



    // sColoredPrint("█", board_pos[0]-5, board_pos[1], YELLOW);
    // sColoredPrint("█", 2, board_pos[1], YELLOW);
    // sColoredPrint("█", 3, board_pos[1], YELLOW);
    // sColoredPrint("█", 4, board_pos[1], YELLOW);
    


    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            if((i==board_pos[0]||i==board_pos[0]+game_board_width+1)&&j>board_pos[1]&&j<board_pos[1]+game_board_height+2){
                sColoredPrint("█", i, j, WHITE);
            }

            if(j==board_pos[1]+1+game_board_height && (i>board_pos[0] && i<board_pos[0]+game_board_width+1)){
                sColoredPrint("█", i, j, WHITE);
            }
        }
    }

    srand(time(NULL));
    new_board = malloc(10*40*sizeof(int8_t));

    last_piece_pos = calloc(8,sizeof(int));
    CreateBoard(game_board_width, game_board_height, game_buffer);
    StartGame();
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
    {'Z', KEY_ROTATECCW},
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
    int piece = GetPiece();
    // if(!piece_was_placed){
    //     // PrintPiece(piece_coords,4,piece_was_placed);
    // }
    int8_t* board = GetCurrBoard();
    int hold = GetCurrHold();
    int* bag = GetCurrBag();

    memcpy(new_board, board, 10*40*sizeof(int8_t));
    for (size_t i = 0; i < 4; i++)
    {
        new_board[piece_coords[i*2] + piece_coords[i*2 + 1] * 10] = piece;
    }

    PrintHold(hold);
    PrintBoard(new_board, 10, 20, 20);
    PrintNext(bag);
}

void PrintNext(int* next){
    static int** old_next_pos = NULL;
    static int* old_next = NULL;
    static int next_count = 4;
    static int h_offset; //horizontal offset
    if (old_next_pos == NULL) {
        old_next = malloc(sizeof(int)*next_count);
        old_next = calloc(next_count,sizeof(int));

        h_offset = 2+game_board_width;
        old_next_pos = malloc(next_count *  sizeof(int*));
        for (size_t i = 0; i < next_count; i++)
        {
            old_next_pos[i] = calloc(4 * 4, sizeof(int));
        }
        return;
    } else {
        for (size_t i = 0; i < next_count; i++)
        {
            if (old_next[i] == next[i]){continue;}
            for (size_t j = 0; j < 4; j++)
            {
                sColoredPrint(" ", old_next_pos[i][j*2]+h_offset, old_next_pos[i][j*2 + 1]-game_board_height+4 + 4*i, YELLOW);
            }
        }
    }


    int** new_pos = malloc(sizeof(int*)*next_count);
    for (size_t i = 0; i < next_count; i++)
    {
        if (old_next[i] == next[i]){continue;}
        int* piece_coords = GetPiecePos(0,0,0,next[i]);
        new_pos[i] = game_to_terminal_coords(piece_coords, 4);
        for (size_t j = 0; j < 4; j++)
        {
            if(old_next_pos[i][piece_coords[j*2] + piece_coords[j*2 + 1] * 4] != 1){
                sColoredPrint("█", new_pos[i][j*2]+ h_offset, new_pos[i][j*2 + 1]-game_board_height+4 +4*i, pieces_colors[next[i]]);
                // sColoredPrint("█", new_pos[i][j*2]+ h_offset, new_pos[i][j*2 + 1]-game_board_height+4 -4*i, pieces_colors[next[0]]);
            }
        }


        memcpy(old_next_pos[i], new_pos[i], 4 * 4 * sizeof(int));
        free(new_pos[i]);
        // return;
    }
    free(new_pos);
}

void PrintHold(int held_piece){
    static int* old_hold = NULL;
    static int old_piece = -1;
    if(old_piece == held_piece){
        return;
    }
    if (old_hold == NULL) {
        old_hold = (int*)calloc(4 * 4, sizeof(int));
    }else{
        for (size_t i = 0; i < 4; i++)
        {
            sColoredPrint(" ", old_hold[i*2]-6, old_hold[i*2 + 1]-game_board_height+4, YELLOW);
        }
    }
    
    int* piece_coords = GetPiecePos(0,0,0,held_piece);
    int* new_pos = game_to_terminal_coords(piece_coords, 4);
    for (size_t i = 0; i < 4; i++)
    {
        if(old_hold[piece_coords[i*2] + piece_coords[i*2 + 1] * 4] != 1){
            sColoredPrint("█", new_pos[i*2]-6, new_pos[i*2 + 1]-game_board_height+4, pieces_colors[held_piece]);
        }
    }

    memcpy(old_hold, new_pos, 4 * 4 * sizeof(int));
    free(new_pos);
    
    // clean up the board for where the old piece was but the new piece isn't


    old_piece = held_piece;
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
                    sColoredPrint("█", new_pos[0], new_pos[1], pieces_colors[board[index]]);
                }else{
                    sColoredPrint(" ", new_pos[0], new_pos[1], BLACK);
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
