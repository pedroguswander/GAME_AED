#include "end_screen.h"
#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "main_menu.h"

#define PLAYER_TEXT_SIZE 32
#define PLAYER_TEXT_SCALE 20

int offset_x = -100;
int start_y = 220;

Vector2 playerSpritePos = {100, 100};

Rectangle _rectSrc = {0, 0, PLAYER_TEXT_SIZE, PLAYER_TEXT_SIZE};
Rectangle _rectDest = {0};

void updateEndScreen()
{
    if (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        _menuOption = MAIN_MENU;
    }
}

void drawEndScreen(Player player)
{
    float t = GetTime();
    int r = 30 + 20 * sinf(t);
    int g = 50 + 20 * sinf(t + 1.0f);
    int b = 70 + 20 * sinf(t + 2.0f);
    ClearBackground((Color){ r, g, b, 255});

    _rectDest = (Rectangle){
        playerSpritePos.x,
        playerSpritePos.y,
        _rectSrc.width * PLAYER_TEXT_SCALE, 
        _rectSrc.height * PLAYER_TEXT_SCALE
    };
    DrawTexturePro(player.sprite, _rectSrc, _rectDest, (Vector2){0, 0}, 0, WHITE);

    DrawText(TextFormat("%s venceu!!", player.name), GetScreenWidth()/2 + offset_x,
    start_y, 80, player.color);

    DrawText("PRESSIONE QUALQUER TECLA PARA VOLTAR", 
            GetScreenWidth()/2 - MeasureText("PRESSIONE QUALQUER TECLA PARA VOLTAR", 32)/2,
            GetScreenHeight()/2 + 300,  // Posiciona abaixo do centro
             40, CLITERAL (Color) {240, 240, 240, 240});
}