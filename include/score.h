#ifndef SCORE_H_
#define SCORE_H_

typedef struct PlayerScore
{
    char nome[50];
    int score;
} PlayerScore;

//void resetScore(void);

#define MAX_NUMBER_OF_PLAYERS 50

void drawScore(int numberOfQuestions);
void addScore(int points);
void resetScore();
int getScore();
void saveScore(char *name);
PlayerScore* loadScores();
#endif