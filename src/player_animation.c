#include <string.h>
#include <stdio.h>
#include "player_animation.h"
#include "raymath.h"

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

static char playerWalkFrontPath[1000] = "player/walk/hero-walk-front/";
static const char *playerWalkFrontFiles[PLAYER_WALK_SHEET_SIZE] = {
    "hero-walk-front-1.png", "hero-walk-front-2.png", "hero-walk-front-3.png",
    "hero-walk-front-4.png", "hero-walk-front-5.png", "hero-walk-front-6.png",
};

static char playerWalkReversePath[1000] = "player/walk/hero-walk-reverse/";
static const char *playerWalkReverseFiles[PLAYER_WALK_SHEET_SIZE] = {
    "hero-walk-reverse-6.png", "hero-walk-reverse-5.png", "hero-walk-reverse-4.png",
    "hero-walk-reverse-3.png", "hero-walk-reverse-2.png", "hero-walk-reverse-1.png",
};

Texture2D playerWalkBackSheet[PLAYER_WALK_SHEET_SIZE];
Texture2D playerWalkSideSheet[PLAYER_WALK_SHEET_SIZE];
Texture2D playerWalkFrontSheet[PLAYER_WALK_SHEET_SIZE];
Texture2D playerWalkReverseSheet[PLAYER_WALK_SHEET_SIZE];

Texture2D playerIdleSprite;
int currentSpriteIndex = 0;
static float changeSpriteTimer = 15.0f/60;


void setPlayerSpriteAnimation(Player *player)
{
    Vector2 targetPos = player->nextTile->position;
    Vector2 direction = Vector2Subtract(targetPos, player->position);
    direction = Vector2Normalize(direction);
    TraceLog(LOG_INFO,"(%f %f)", direction.x, direction.y);

    float dx = direction.x;
    float dy = direction.y;
    
    // Tolerância para considerar movimento horizontal/vertical
    const float tolerance = 0.3f;
    
    if (fabs(dy) < tolerance)  // Movimento principalmente horizontal
    {
        player->sprite = playerWalkSideSheet[currentSpriteIndex];
        // Aplica flip horizontal se estiver indo para a esquerda
        if (dx < 0) 
        {
            player->sprite = playerWalkReverseSheet[currentSpriteIndex];
            player->flipHorizontal = true;
        }
        else
        {
            player->flipHorizontal = false;
        }
    }
    else if (dy < -tolerance)  // Movimento para cima
    {
        player->sprite = playerWalkBackSheet[currentSpriteIndex];
        player->flipHorizontal = false;
    }
    else if (dy > tolerance)   // Movimento para baixo
    {
        player->sprite = playerWalkFrontSheet[currentSpriteIndex];
        player->flipHorizontal = false;
    }
}

void setSpriteToIdle(Player *player)
{
    player->sprite = playerIdleSprite;
}

void InitPlayerAnimation() {
    currentSpriteIndex = 0;

    char filePath[1024];  // Declarado uma única vez fora do loop

    for (int i = 0; i < PLAYER_WALK_SHEET_SIZE; i++) {
        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkBackPath, playerWalkBackFiles[i]);
        playerWalkBackSheet[i] = LoadTexture(filePath);

        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkSidePath, playerWalkSideFiles[i]);
        playerWalkSideSheet[i] = LoadTexture(filePath);

        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkFrontPath, playerWalkFrontFiles[i]);
        playerWalkFrontSheet[i] = LoadTexture(filePath);

        snprintf(filePath, sizeof(filePath), "%s%s", playerWalkReversePath, playerWalkReverseFiles[i]);
        playerWalkReverseSheet[i] = LoadTexture(filePath);
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
        UnloadTexture(playerWalkFrontSheet[i]);
    }
}

void ChangePlayerWalkAnimationSprite() {
    currentSpriteIndex++;
    if (currentSpriteIndex >= PLAYER_WALK_SHEET_SIZE) {
        currentSpriteIndex = 0;
    }
}