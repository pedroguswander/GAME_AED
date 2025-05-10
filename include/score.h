#ifndef SCORE_H_
#define SCORE_H_

typedef struct PlayerScore
{
    char nome[50];
    int score;
    int existe;
} PlayerScore;

//void resetScore(void);

#define MAX_NUMBER_OF_PLAYERS 50

void drawScore(int numberOfQuestions);
void addScore(int points);
void resetScore();
int getScore();
void saveScore(char *name);
void ordernarScore(int current_number_of_players);
PlayerScore* loadScores();
#endif