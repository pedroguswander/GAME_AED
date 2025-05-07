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
#include "question.h"
#include "topics.h"

typedef enum {    
    QUESTION_SCREEN=0,
    ANSWER_SCREEN=1,
    FINAL_SCORE_SCREEN=2,
} GameState;

GameState _state = QUESTION_SCREEN;
char *_current_topic = "Algoritmos e Estruturas de Dados"; //escolha o tópico
bool _gotItRight = false;
int _currentQuestion = 0;


void checkIfAnswerIsRight(Option *options, Question question);

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    Rectangle nextQuestionButton = {screenWidth/4, screenHeight-100, 480, 320};
    const char (*themes)[100] = getThemesOfTopic(_current_topic);


    InitWindow(screenWidth, screenHeight, "Quiz Game");

    Question questions[5];
    for (int i = 0; i < 5; i++)
    {
        questions[i] = addQuestion(_current_topic, themes[i]);
    }

    Option options[4];
    char *labels[] = {"A", "B", "C", "D"};
    Rectangle optionRects[4];
    int startY = 100;

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40 };
        options[i].rect = optionRects[i];
        strcpy(options[i].answer, labels[i]);
    }

    while (!WindowShouldClose()) {
        //UPTADE
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _state == QUESTION_SCREEN) {
            checkIfAnswerIsRight(options, questions[_currentQuestion]);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
            if (_currentQuestion >= 4) { // Mudar de 2 para 4 (0-4 = 5 questões)
                _state = FINAL_SCORE_SCREEN;
            } else {
                _state = QUESTION_SCREEN;
                _currentQuestion++;
            }
            _gotItRight = false;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);

        drawScore(5);
        //
        switch (_state)
        {

            case QUESTION_SCREEN:
                drawQuestion(options, questions[_currentQuestion]);
                break;

            case ANSWER_SCREEN: 
                //adicionar função drawAnswer em outro arquivo
                DrawText("Gabarito:", 50, 100, 28, YELLOW);
                DrawText(questions[_currentQuestion].answer, 200, 100, 28, WHITE);

                if (_gotItRight) {
                    DrawText("ACERTOU!", 50, 200, 40, GREEN);

                } else {
                    DrawText("ERROU!", 50, 200, 40, RED);
                }
                DrawRectangleRec(nextQuestionButton, RED);
                DrawText("CONTINUAR",nextQuestionButton.x+5,nextQuestionButton.y+5, 16 ,WHITE);
                break;

            case FINAL_SCORE_SCREEN:
                DrawText("QUIZ CONCLUÍDO!", screenWidth/2 - MeasureText("QUIZ CONCLUÍDO!", 40)/2, 100, 40, YELLOW);
                DrawText(TextFormat("Pontuação Final: %d/%d", getScore(), 5), 
                        screenWidth/2 - MeasureText("Pontuação Final: 0/5", 30)/2, 200, 30, WHITE);
                
                // Botão para reiniciar
                DrawRectangleRec(nextQuestionButton, GREEN);
                DrawText("JOGAR NOVAMENTE", nextQuestionButton.x+5, nextQuestionButton.y+5, 16, WHITE);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void checkIfAnswerIsRight(Option *options, Question question) {
    Vector2 mouse = GetMousePosition();
    _state = ANSWER_SCREEN;
    _gotItRight = false;

    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointRec(mouse, options[i].rect)) {
            if (strcmp(options[i].answer, question.answer) == 0) {
                _gotItRight = true;
                addScore(1);
            }
            return;
        }
    }
}
