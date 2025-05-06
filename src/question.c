#include "question.h"

Question createQuestion(char *statement, char *optionA, char *optionB, char *optionC, char *optionD, char *answer)
{
    Question question = {
        .statement = statement,
        .optionA = optionA,
        .optionB = optionB,
        .optionC = optionC,
        .optionD = optionD,
        .answer = answer
    };

    return question;
}

void drawQuestion(Option *options, Question question)
{
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


