#include "question.h"
#include "topics.h"
#include "prompt.h"
#include <stdlib.h>
#include <string.h>

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC, char *optionD, char *answer)
{
    Question question;

    question.statement = strdup(statement);
    question.optionA = strdup(optionA);
    question.optionB = strdup(optionB);
    question.optionC = strdup(optionC);
    question.optionD = strdup(optionD);
    question.answer = strdup(answer);

    return question;
}

void freeQuestion(Question question) {
    free(question.statement);
    free(question.optionA);
    free(question.optionB);
    free(question.optionC);
    free(question.optionD);
    free(question.answer);
}

void drawQuestion(Option *options, Question question)
{
    printf("\n================ PERGUNTA ================\n");
    printf("%s\n", question.statement);
    printf("A) %s\n", question.optionA);
    printf("B) %s\n", question.optionB);
    printf("C) %s\n", question.optionC);
    printf("D) %s\n", question.optionD);
    printf("==========================================\n\n");
    DrawText(question.statement, 0, 0, 32, WHITE);

    for (int i = 0; i < 4; i++) { //funcao
        Color rectColor = DARKGRAY;
        if (CheckCollisionPointRec(GetMousePosition(), options[i].rect)) {
            rectColor = LIGHTGRAY;
        }
    
        DrawRectangleRec(options[i].rect, rectColor);
    
        const char *optionText;
        switch (i) {
            case 0: optionText = question.optionA; break;
            case 1: optionText = question.optionB; break;
            case 2: optionText = question.optionC; break;
            case 3: optionText = question.optionD; break;
        }
    
        DrawText(optionText, options[i].rect.x + 10, options[i].rect.y + 10, 20, WHITE);
    }
}



