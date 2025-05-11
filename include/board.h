#ifndef BOARD_H_
#define BOARD_H_
#include "raylib.h"

typedef enum {
    CAN_PLAY = 0,
    MOVING =1,
    EVENT = 2,
} BoardState;

typedef struct {
    Vector2 position;
    Vector2 prevPosition;
    int currentTile;
    int prevTile;
} Player;

typedef enum {
    QUESTION=0,
    BOSS=1,
    SPECIAL_EVENT=2,
} TileType;

typedef struct {
    Vector2 position;
    TileType type;
    char topic[100];
} Tile;

#define TILE_DISTANCE 150
#define NUMBER_OF_TILES 3

void updateBoard(void);
void drawBoard(void);
void createBoard(void);

#endif