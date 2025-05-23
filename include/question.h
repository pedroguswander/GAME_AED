#ifndef QUESTION_H_
#define QUESTION_H_
#include "raylib.h"

typedef struct Question
{
    char *statement;
    char *optionA;
    char *optionB;
    char *optionC;
    char *optionD;
    char *answer;
} Question;

typedef struct Option
{
    Rectangle rect;
    Color color;
	char answer[2];
    int clicked;
} Option;

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC,char *optionD, char *anwser);
void drawQuestion(Question question, bool answered);
void freeQuestion(Question question);
void createOptions(void);
void UpdateOptions(Question question);
bool checkIfAnsewered(Question question, bool *gotItRight);

#endif