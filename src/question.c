#include "question.h"
#include "topics.h"
#include "prompt.h"
#include <stdlib.h>
#include <string.h>

Option _options[4];
Rectangle optionRects_[4];
int startOptionsRectY = 100;
char *labels_[] = {"A", "B", "C", "D"};

void createOptions()
{
    for (int i = 0; i < 4; i++) {
        optionRects_[i] = (Rectangle){ 50, startOptionsRectY + i * 60, 800, 40};
        _options[i].rect = optionRects_[i];
        _options[i].color = RED;
        strcpy(_options[i].answer, labels_[i]);
    }
}

void UpdateOptions(Question question)
{
    for (int i = 0; i < 4; i++) {
        if (strcmp(_options[i].answer, question.answer) == 0)
        {
            _options[i].color = GREEN;
        }
        else
        {
            _options[i].color = RED;
        }

    }
}

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

void drawQuestion(Question question)
{
    printf("\n================ PERGUNTA ================\n");
    printf("%s\n", question.statement);
    printf("A) %s\n", question.optionA);
    printf("B) %s\n", question.optionB);
    printf("C) %s\n", question.optionC);
    printf("D) %s\n", question.optionD);
    printf("==========================================\n\n");
    DrawText(question.statement, 5, 10, 32, WHITE);

    for (int i = 0; i < 4; i++) { //funcao
        Color rectColor = DARKGRAY;
        if (CheckCollisionPointRec(GetMousePosition(), _options[i].rect)) {
            rectColor = LIGHTGRAY;
        }
    
        const char *optionText;
        switch (i) {
            case 0: optionText = question.optionA; break;
            case 1: optionText = question.optionB; break;
            case 2: optionText = question.optionC; break;
            case 3: optionText = question.optionD; break;
        }
        
        DrawRectangleRec(_options[i].rect, _options[i].color);

        DrawText(optionText, _options[i].rect.x + 10, _options[i].rect.y + 10, 20, WHITE);


    }
}



