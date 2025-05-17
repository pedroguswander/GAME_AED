//
// Created by antonio on 16/05/25.
//

#include "raylib.h"
#include "player_names.h"
#include <string.h>
#include <math.h>

char player1Name[MAX_NAME_LENGTH] = "";
char player2Name[MAX_NAME_LENGTH] = "";

void showPlayerNameInputScreen() {
    char inputText[MAX_NAME_LENGTH] = "";
    int currentPlayer = 1;
    bool nameEntered = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    while (!nameEntered && !WindowShouldClose()) {
        BeginDrawing();

        // Fundo animado
        float t = GetTime();
        int r = 50 + 50 * sinf(t);
        int g = 100 + 50 * sinf(t + 1.0f);
        int b = 150 + 50 * sinf(t + 2.0f);
        ClearBackground((Color){ r, g, b, 255 });

        // Bolhas subindo
        for (int i = 0; i < 30; i++) {
            float y = (float)(screenHeight - fmod(t * 40 + i * 60, screenHeight));
            float x = 80 + (i * 60) % screenWidth;
            DrawCircle(x, y, 6, Fade(WHITE, 0.2f));
        }

        // Título
        const char* prompt = TextFormat("Digite o nome do Player %d:", currentPlayer);
        int promptWidth = MeasureText(prompt, 30);
        DrawText(prompt, screenWidth/2 - promptWidth/2, screenHeight/2 - 80, 30, YELLOW);

        // Campo de texto
        int boxWidth = 400;
        int boxHeight = 50;
        int boxX = screenWidth/2 - boxWidth/2;
        int boxY = screenHeight/2;
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, WHITE);
        DrawText(inputText, boxX + 10, boxY + 15, 20, BLACK);

        EndDrawing();

        int key = GetCharPressed();
        if (key >= 32 && key <= 125 && strlen(inputText) < MAX_NAME_LENGTH - 1) {
            int len = strlen(inputText);
            inputText[len] = (char)key;
            inputText[len + 1] = '\0';
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(inputText);
            if (len > 0) inputText[len - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (currentPlayer == 1) {
                strcpy(player1Name, inputText);
                currentPlayer = 2;
                inputText[0] = '\0'; // limpa campo
            } else {
                strcpy(player2Name, inputText);
                nameEntered = true;
            }
        }
    }
}
