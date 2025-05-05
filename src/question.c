#include "question.h"

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC, char *optionD, char *answer)
{
    Question question = {
        .statement = statement,
        .optionA = optionA,
        .optionB = optionB,
        .optionC = optionC,
        .optionD = optionD,
        .anwser = answer
    };

    return question;
}