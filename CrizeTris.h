#include "GameUtils.h"





#define KEY_RIGHT 0x0001
#define KEY_LEFT 0x0002
#define KEY_UP 0x0004
#define KEY_DOWN 0x0008
#define KEY_HARDDROP 0x0010
#define KEY_ROTATECW 0x0020
#define KEY_ROTATECCW 0x0040
#define KEY_HOLD 0x0080
#define KEY_180 0x0100

void CreateBoard(int width, int height, int buffer);
void PlacePiece(int x, int y, int8_t piece, int rotation);
void SpawnPiece();
void StartGame();
bool MovePiece(enum direction direction);
bool GameLoop(int16_t input);
void RotatePiece(enum rotations rotation);