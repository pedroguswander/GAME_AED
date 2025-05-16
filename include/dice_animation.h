#ifndef DICE_ANIMATION_H_
#define DICE_ANIMATION_H_
#include "raylib.h"

#define DICE_SIZE 16
#define DICES 6
#define DICE_TIME 5.0f/ 60 // fps do dado* fps do
#define DICE_SCALE 7

void initDice();
void drawDice();
void updateDice();
void setDiceResult(int result);
void changeDiceSprite();

#endif