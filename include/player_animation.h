#ifndef PLAYER_ANIMATION_H
#define PLAYER_ANIMATION_H

#include "raylib.h"
#include "board.h"

#define PLAYER_WALK_SHEET_SIZE 6

void InitPlayerAnimation();
void UpdatePlayerAnimation();
void UnloadPlayerAnimation();
void ChangePlayerWalkAnimationSprite();
void setPlayerSpriteAnimation(Player *player, int index);
void setSpriteToIdle(Player *player, int index);

#endif