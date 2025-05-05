#ifndef QUESTION_H_
#define QUESTION_H_

typedef struct Question
{
    char *statement;
    char *optionA;
    char *optionB;
    char *optionC;
    char *optionD;
    char *anwser;
} Question;

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC,char *optionD, char *anwser);
#endif