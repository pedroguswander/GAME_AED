#include "player_animation.h"
#include <string.h>

static char playerWalkBackPath[1000] = "player/walk/hero-walk-back/";
static const char *playerWalkBackFiles[PLAYER_WALK_BACK_SHEET_SIZE] = {
    "hero-walk-back-1.png", "hero-walk-back-2.png", "hero-walk-back-3.png",
    "hero-walk-back-4.png", "hero-walk-back-5.png", "hero-walk-back-6.png",
};

Texture2D playerWalkBackSheet[PLAYER_WALK_BACK_SHEET_SIZE];
Texture2D playerIdleSprite;
int currentSpriteIndex = 0;
static float changeSpriteTimer = 1.0f/30;

void InitPlayerAnimation() {
    for (int i = 0; i < PLAYER_WALK_BACK_SHEET_SIZE; i++) {
        char filePath[1024];
        strcpy(filePath, playerWalkBackPath);
        strcat(filePath, playerWalkBackFiles[i]);
        playerWalkBackSheet[i] = LoadTexture(filePath);
        strcpy(playerWalkBackPath, "player/walk/hero-walk-back/");
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
    for (int i = 0; i < PLAYER_WALK_BACK_SHEET_SIZE; i++) {
        UnloadTexture(playerWalkBackSheet[i]);
    }
}

void ChangePlayerWalkAnimationSprite() {
    currentSpriteIndex++;
    if (currentSpriteIndex >= PLAYER_WALK_BACK_SHEET_SIZE) {
        currentSpriteIndex = 0;
    }
}