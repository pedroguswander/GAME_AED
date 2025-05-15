#include "player_animation.h"
#include <string.h>
#include <stdio.h>

static char playerWalkBackPath[1000] = "player/walk/hero-walk-back/";
static const char *playerWalkBackFiles[PLAYER_WALK_SHEET_SIZE] = {
    "hero-walk-back-1.png", "hero-walk-back-2.png", "hero-walk-back-3.png",
    "hero-walk-back-4.png", "hero-walk-back-5.png", "hero-walk-back-6.png",
};


static char playerWalkSidePath[1000] = "player/walk/hero-walk-side/";
static const char *playerWalkSideFiles[PLAYER_WALK_SHEET_SIZE] = {
    "hero-walk-side-1.png", "hero-walk-side-2.png", "hero-walk-side-3.png",
    "hero-walk-side-4.png", "hero-walk-side-5.png", "hero-walk-side-6.png",
};


Texture2D playerWalkBackSheet[PLAYER_WALK_SHEET_SIZE];
Texture2D playerWalkSideSheet[PLAYER_WALK_SHEET_SIZE];

Texture2D playerIdleSprite;
int currentSpriteIndex = 0;
static float changeSpriteTimer = 1.0f/30;

void InitPlayerAnimation() {
    char filePath[1024];  // Declarado uma única vez fora do loop

    for (int i = 0; i < PLAYER_WALK_SHEET_SIZE; i++) {
        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkBackPath, playerWalkBackFiles[i]);
        playerWalkBackSheet[i] = LoadTexture(filePath);

        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkSidePath, playerWalkSideFiles[i]);
        playerWalkSideSheet[i] = LoadTexture(filePath);
    }

    playerIdleSprite = LoadTexture("player/hero-idle-front.png");
}

void UpdatePlayerAnimation() {
    changeSpriteTimer -= GetFrameTime();
    if (changeSpriteTimer <= 0) {
        ChangePlayerWalkAnimationSprite();
        changeSpriteTimer = 1.0f/30;
    }
}

void UnloadPlayerAnimation() {
    for (int i = 0; i < PLAYER_WALK_SHEET_SIZE; i++) {
        UnloadTexture(playerWalkBackSheet[i]);
        UnloadTexture(playerWalkSideSheet[i]);
    }
}

void ChangePlayerWalkAnimationSprite() {
    currentSpriteIndex++;
    if (currentSpriteIndex >= PLAYER_WALK_SHEET_SIZE) {
        currentSpriteIndex = 0;
    }
}