#include "dice_animation.h"

Texture2D diceSpriteSheet;
Rectangle diceRectSrc = {0};
Rectangle  diceRectDest = {783, 129, DICE_SIZE* DICE_SCALE, DICE_SIZE * DICE_SCALE};
int currentDiceIndex = 0;
float diceTimer = DICE_TIME;

void setDiceResult(int result) //agora o drawDice vai mostrar o sprite correspondende ao reusltado
{
    currentDiceIndex = result - 1;
    changeDiceSprite();
}

void initDice()
{
    diceSpriteSheet = LoadTexture("objects/dice.png");
}

void changeDiceSprite()
{
    diceRectSrc = (Rectangle) {DICE_SIZE * currentDiceIndex, 0, DICE_SIZE, DICE_SIZE};
}
void updateDice()
{
    diceTimer -= GetFrameTime();
    if (diceTimer <= 0)
    {
        currentDiceIndex++;
        if (currentDiceIndex >= DICES)
        {
            currentDiceIndex = 0;
        }

        diceTimer = DICE_TIME;
    }

    changeDiceSprite();
}

void drawDice()
{
    DrawTexturePro(diceSpriteSheet, diceRectSrc, diceRectDest, (Vector2) {0, 0}, 0, WHITE);
}