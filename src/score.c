#include "score.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "game.h"

static int _score = 0;
PlayerScore players[MAX_NUMBER_OF_PLAYERS] = {0};

void addScore(int points)
{
    _score+= points;
}

void resetScore()
{
    _score = 0;
}

int getScore()
{
    return _score;
}

void ordernarScore(int current_number_of_players)
{
    for (int i = 1; i <= current_number_of_players; i++) {
        int j = i;
        while (j > 0 && players[j].score > players[j-1].score) {
            PlayerScore aux = players[j];
            players[j] = players[j-1];
            players[j-1] = aux;  // Corrigido: estava players[j-1] = players[j]
            j--;
        }
    }
}

void saveScore(char *name) {
    // Carrega os scores existentes
    int k = 0;
    FILE *f = fopen("hall.txt", "r");
    if (f != NULL) {
        while (k < MAX_NUMBER_OF_PLAYERS && 
               fscanf(f, "%20s %d", players[k].nome, &players[k].score) == 2) {
            players[k].existe = 1;
            k++;
        }
        fclose(f);
    }

    // Adiciona o novo jogador
    if (k < MAX_NUMBER_OF_PLAYERS) {
        strncpy(players[k].nome, name, sizeof(players[k].nome) - 1);
        players[k].nome[sizeof(players[k].nome) - 1] = '\0';
        players[k].score = _score;
        players[k].existe = 1;
        k++;
    }

    for (int i = 1; i <= k; i++) {
        int j = i;
        while (j > 0 && players[j].score > players[j-1].score) {
            PlayerScore aux = players[j];
            players[j] = players[j-1];
            players[j-1] = aux;  // Corrigido: estava players[j-1] = players[j]
            j--;
        }
    }

    // Salva de volta no arquivo
    f = fopen("hall.txt", "w");
    if (f == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int j = 0; j < k; j++) {
        fprintf(f, "%s %d\n", players[j].nome, players[j].score);
    }

    fclose(f);
}

PlayerScore* loadScores() {
    memset(players, 0, sizeof(players));
    
    FILE *f = fopen("hall.txt", "r");
    if (f == NULL) {
        return players; // Retorna array vazio se o arquivo não existir
    }

    int count = 0;
    while (count < MAX_NUMBER_OF_PLAYERS && 
           fscanf(f, "%20s %d", players[count].nome, &players[count].score) == 2) {
        players[count].existe = 1;
        count++;
    }

    fclose(f);
    return players;
}

void drawScore(int numberOfQuestions)
{
    const char *_scoreText = TextFormat("SCORE: %d/%d", _score, numberOfQuestions);
    DrawText(_scoreText, 400, 400, 16, WHITE);
}