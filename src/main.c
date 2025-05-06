#include "raylib.h"
#include "resource_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "api.h"
#include "prompt.h"
#include "score.h"

typedef struct Temp {
    Rectangle rect;
    char optionAnswer[2];
} Temp;

typedef enum {    
    QUESTION_SCREEN=0,
    ANSWER_SCREEN=1,
} GameState;

GameState _state = QUESTION_SCREEN;
bool _gotItRight = false;
int _currentQuestion = 0;

void checkIfAnswerIsRight(Temp *temp, Question question);
void drawQuestion(Temp *options, Question question);

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    Rectangle nextQuestionButton = {screenWidth/4, screenHeight-100, 480, 320};

    InitWindow(screenWidth, screenHeight, "Quiz Game");

    //Question question = addQuestion();
    Question questions[2];
    for (int i = 0; i < 2; i++)
    {
        questions[i] = addQuestion();
    }

    Temp options[4];
    char *labels[] = { "A", "B", "C", "D" };
    Rectangle optionRects[4];
    int startY = 100;

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40 };
        options[i].rect = optionRects[i];
        strcpy(options[i].optionAnswer, labels[i]);
    }

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _state == QUESTION_SCREEN) {
            checkIfAnswerIsRight(options, questions[_currentQuestion]);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
            _state = QUESTION_SCREEN;
            _gotItRight = false;
            if(_currentQuestion >= 2) { _currentQuestion = 0; resetScore();}
            else { _currentQuestion++; }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        drawScore(2);

        switch (_state)
        {

            case QUESTION_SCREEN:
                drawQuestion(options, questions[_currentQuestion]);
                break;

            case ANSWER_SCREEN: //drawAnswer
                DrawText("Gabarito:", 50, 100, 28, YELLOW);
                DrawText(questions[_currentQuestion].answer, 200, 100, 28, WHITE);

                if (_gotItRight) {
                    DrawText("ACERTOU!", 50, 200, 40, GREEN);

                } else {
                    DrawText("ERROU!", 50, 200, 40, RED);
                }
                DrawRectangleRec(nextQuestionButton, RED);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void checkIfAnswerIsRight(Temp *temp, Question question) {
    Vector2 mouse = GetMousePosition();
    _state = ANSWER_SCREEN;
    _gotItRight = false;

    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointRec(mouse, temp[i].rect)) {
            if (strcmp(temp[i].optionAnswer, question.answer) == 0) {
                _gotItRight = true;
                addScore(1);
            }
            return;
        }
    }
}

void drawQuestion(Temp *options, Question question)
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
