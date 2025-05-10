#include "hall.h"
#include "score.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

static PlayerScore *players_of_hall;

void uptadeHallOfFame() {
    players_of_hall = loadScores();
}

void drawHallOfFame() {
    ClearBackground(BLACK);
    DrawText("HALL DA FAMA", 500, 50, 40, YELLOW);

    if (players_of_hall == NULL) {
        DrawText("Nenhum dado disponível.", 500, 150, 20, WHITE);
        return;
    }

    for (int i = 0; i < 5 && i < MAX_NUMBER_OF_PLAYERS && players_of_hall[i].existe; i++) {
        DrawText(TextFormat("%d. %s - %d pontos", i + 1, players_of_hall[i].nome, players_of_hall[i].score),
                400, 150 + i * 40, 24, LIGHTGRAY);
    }
}
