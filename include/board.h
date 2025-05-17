#ifndef BOARD_H_
#define BOARD_H_
#include "raylib.h"

#define TILE_DISTANCE 150
#define NAME_LENGTH 9

typedef enum {
    CAN_PLAY_TRANSITION,
    CAN_PLAY,
    MOVING,
    MOVING_BACKWARDS,
    LOADING, 
    EVENT,
    SHOW_ANSWER,
    WAITING_TO_END,
    END
} BoardState;

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

typedef struct {
    Vector2 position;
    Vector2 prevPosition;
    Tile *currentTile;
    Tile *nextTile;
    Tile *prevTile;
    Tile *targetTile;
    int number;
    bool flipHorizontal;
    Color color;
    BoardState state;
    Texture2D sprite;
    char name[NAME_LENGTH];
} Player;

void createTile(TileType type, const char *topic, int casa);
Vector2 getPositionOfTile(int casa);
Tile *getTileByTile(int tile);
bool movePlayer(Player *player, bool forward);
void finalizeTurn(void);
void updateBoard(void);
void drawBoard(void);
void createBoard(void);
void changePlayerWalkAnimationSprite(void);
void drawPlayer(Player *p);
void freeBoard();
void resetBoard();

#endif