#include "score.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
//#include "game.h"

static int _score = 0;

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

void saveScore(char *name)
{
    FILE *f = NULL;
    PlayerScore players[100];
    int i = 0;
    f = fopen("hall.txt", "r");
    while (fscanf(f, "%20s %d", players[i].nome, &players[i].score) == 2)
    {
        i++;
    }
    
    fclose(f);

    PlayerScore new_player;
    strcpy(new_player.nome, name);
    new_player.score = _score;
    if (i < 100) {
        players[i++] = new_player;
    }

    f = fopen("hall.txt", "w");
    if (f == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i=1; i <= 50; i++)
    {
        int j =i;
        while(players[j].score > players[j-1].score && j > 0)
        {
            PlayerScore aux = players[j];
            players[j] = players[j-1];
            players[j-1] = players[j];
            j--;
        }
    }

    for (int j = 0; j < i; j++) {
        fprintf(f, "%s %d\n", players[j].nome, players[j].score);
    }
}

PlayerScore* loadScores() {
    FILE *file = fopen("hall.txt", "r");
    if (!file) return NULL;

    PlayerScore *players = NULL;
    PlayerScore temp;
    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99s %d", temp.nome, &temp.score) == 2) {
            players = (PlayerScore*)realloc(players, (count + 1) * sizeof(PlayerScore));
            players[count++] = temp;
        }
    }
    

    fclose(file);
    return players;
}

void drawScore(int numberOfQuestions)
{
    const char *_scoreText = TextFormat("SCORE: %d/%d", _score, numberOfQuestions);
    DrawText(_scoreText, 400, 400, 16, WHITE);
}