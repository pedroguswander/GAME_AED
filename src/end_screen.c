#include "end_screen.h"
#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "main_menu.h"

#define PLAYER_TEXT_SIZE 32
#define PLAYER_TEXT_SCALE 20

int offset_x = 100;
int start_y = 180;

Vector2 playerSpritePos = {100, 100};

Rectangle _rectSrc = {0, 0, PLAYER_TEXT_SIZE, PLAYER_TEXT_SIZE};
Rectangle _rectDest = {0};

void updateEndScreen()
{
    Rectangle menuButton = {
        GetScreenWidth()/2 - MeasureText("MENU", 48)/2,
        GetScreenHeight()/2 + 200,  // Posiciona abaixo do centro
        (float)MeasureText("MENU", 48),
        48
    };

    if (CheckCollisionPointRec(GetMousePosition(), menuButton))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            _menuOption = MAIN_MENU; // Certifique-se que MAIN_MENU está definido em main_menu.h
            TraceLog(LOG_INFO, "Mudando para menu principal"); // Debug
        }
    }
}

void drawEndScreen(Player player)
{
    float t = GetTime();
    int r = 30 + 20 * sinf(t);
    int g = 50 + 20 * sinf(t + 1.0f);
    int b = 70 + 20 * sinf(t + 2.0f);
    ClearBackground((Color){ r, g, b, 255 });

    _rectDest = (Rectangle){
        playerSpritePos.x,
        playerSpritePos.y,
        _rectSrc.width * PLAYER_TEXT_SCALE, 
        _rectSrc.height * PLAYER_TEXT_SCALE
    };
    DrawTexturePro(player.sprite, _rectSrc, _rectDest, (Vector2){0, 0}, 0, WHITE);

    DrawText(TextFormat("%s venceu!!", player.name), GetScreenWidth()/2 + offset_x,
    start_y, 64, player.color);

    Rectangle menuButton = {
        GetScreenWidth()/2 - 150,
        GetScreenHeight()/2 + 250,
        300,
        80
    };
    
    // Verifica se o mouse está sobre o botão
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), menuButton);
    
    // Cor do botão (muda quando hover)
    Color btnColor = DARKGRAY;
    Color textColor = WHITE;
    
    if (isHovered)
    {
        DrawRectangleRounded(menuButton, 0.5f, 10, btnColor);
    }

    DrawText("MENU", 
             menuButton.x + menuButton.width/2 - MeasureText("MENU", 40)/2,
             menuButton.y + menuButton.height/2 - 20, 
             40, textColor);
}