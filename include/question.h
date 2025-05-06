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
	char answer[2];
} Option;

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC,char *optionD, char *anwser);
#endif