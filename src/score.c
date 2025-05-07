#include "score.h"
#include "raylib.h"
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

void drawScore(int numberOfQuestions)
{
    const char *_scoreText = TextFormat("SCORE: %d/%d", _score, numberOfQuestions);
    DrawText(_scoreText, 400, 400, 16, WHITE);
}