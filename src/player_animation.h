#ifndef PLAYER_ANIMATION_H
#define PLAYER_ANIMATION_H

#include "raylib.h"

#define PLAYER_WALK_SHEET_SIZE 6

extern Texture2D playerWalkBackSheet[PLAYER_WALK_SHEET_SIZE];
extern Texture2D playerWalkSideSheet[PLAYER_WALK_SHEET_SIZE];

extern Texture2D playerIdleSprite;
extern int currentSpriteIndex;

void InitPlayerAnimation();
void UpdatePlayerAnimation();
void UnloadPlayerAnimation();
void ChangePlayerWalkAnimationSprite();

#endif