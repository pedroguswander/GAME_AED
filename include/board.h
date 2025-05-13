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

typedef struct Tile {
    Vector2 position;
    TileType type;
    Rectangle rect;
    char topic[100];
    int tile;
    struct Tile *next;
    struct Tile *prev;
} Tile;

#define TILE_DISTANCE 150
#define NUMBER_OF_TILES 3

void createTile(TileType type, const char *topic, int casa);
Vector2 getPositionOfTile(int casa);
void updateBoard(void);
void drawBoard(void);
void createBoard(void);
void freeBoard();



#endif