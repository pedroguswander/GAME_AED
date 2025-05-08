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
#include <pthread.h>

typedef enum {
    MAIN_MENU_SCREEN = 0,
    TOPIC_SELECTION_SCREEN = 1,
    LOADING_SCREEN = 2,
    QUESTION_SCREEN = 3,
    ANSWER_SCREEN = 4,
    FINAL_SCORE_SCREEN = 5,
} GameState;

GameState _state = MAIN_MENU_SCREEN;
char *_current_topic = "Algoritmos e Estruturas de Dados"; //escolha o tópico
bool _gotItRight = false;
bool _quizLoaded = false;
int _currentQuestion = 0;
Question questions[5];
pthread_t loaderThread;
bool _loadingFinished = false;

void checkIfAnswerIsRight(Option *options, Question question);
void *loadQuestionsThread(void *arg);

int main() {
    PlayerScore *buffer = NULL;
    const int screenWidth = 1280;
    const int screenHeight = 800;
    const char *topics[] = {
        "Algoritmos e Estruturas de Dados",
        "INFRA_SO",
        "POO",
        "Historia do Brasil"
    };
    
    Rectangle topicButtons[4];
    for (int i = 0; i < 4; i++) {
        topicButtons[i] = (Rectangle){ screenWidth/2 - 200, 150 + i*100, 400, 60 };
    }

    Rectangle mainMenuButtons[4];
    const char *mainMenuLabels[] = {
        "Modo Normal",
        "Modo Tabuleiro",
        "Hall da Fama",
        "Créditos"
    };
    for (int i = 0; i < 4; i++) {
        mainMenuButtons[i] = (Rectangle){ screenWidth/2 - 200, 200 + i*100, 400, 60 };
    }

    Rectangle nextQuestionButton = {screenWidth/4, screenHeight-100, 480, 320};

    InitWindow(screenWidth, screenHeight, "Quiz Game");

    Option options[4];
    char *labels[] = {"A", "B", "C", "D"};
    Rectangle optionRects[4];
    int startY = 100;

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40};
        options[i].rect = optionRects[i];
        strcpy(options[i].answer, labels[i]);
    }

    while (!WindowShouldClose()) {
        //UPTADE
        if (_state == MAIN_MENU_SCREEN && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mouse, mainMenuButtons[i])) {
                    switch(i) {
                        case 0: // Modo Normal
                            _state = TOPIC_SELECTION_SCREEN;
                            break;
                        case 1: // Modo Tabuleiro
                            // Implementar depois
                            break;
                        case 2: // Hall da Fama
                            // Implementar depois
                            break;
                        case 3: // Créditos
                            // Implementar depois
                            break;
                    }
                }
            }
        }
        

        if (_state == TOPIC_SELECTION_SCREEN && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 4; i++) { //uptadeTopics
                if (CheckCollisionPointRec(GetMousePosition(), topicButtons[i])) {
                    _current_topic = (char *)topics[i];
                    resetScore();
                    _currentQuestion = 0;
                    _loadingFinished = false;
                    _state = LOADING_SCREEN;
                    
                    // Cria uma thread para carregar perguntas
                    pthread_create(&loaderThread, NULL, loadQuestionsThread, (void *)_current_topic);
                    break;
                }
            }
        }

        if (_state == LOADING_SCREEN)
        {
            if (_loadingFinished) {
                _state = QUESTION_SCREEN;
            }
        }

        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _state == QUESTION_SCREEN) {
            checkIfAnswerIsRight(options, questions[_currentQuestion]);
        }

        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
            if (_state == FINAL_SCORE_SCREEN) {
                _state = TOPIC_SELECTION_SCREEN;
            } else if (_state == ANSWER_SCREEN) {
                _currentQuestion++;
                if (_currentQuestion >= 5) {
                    _state = FINAL_SCORE_SCREEN;
                } else {
                    _state = QUESTION_SCREEN;
                }
                _gotItRight = false;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        switch (_state)
        {
            case MAIN_MENU_SCREEN:
                DrawText("MENU PRINCIPAL", screenWidth/2 - MeasureText("MENU PRINCIPAL", 40)/2, 80, 40, YELLOW);
                for (int i = 0; i < 4; i++) {
                    DrawRectangleRec(mainMenuButtons[i], DARKGRAY);
                    DrawText(mainMenuLabels[i], mainMenuButtons[i].x + 20, mainMenuButtons[i].y + 15, 24, WHITE);
                }
                break;

            case TOPIC_SELECTION_SCREEN:
            DrawText("Selecione um Tópico", screenWidth/2 - MeasureText("Selecione um Tópico", 30)/2, 50, 30, YELLOW);
            for (int i = 0; i < 4; i++) {
                DrawRectangleRec(topicButtons[i], GRAY);
                DrawText(topics[i], topicButtons[i].x + 10, topicButtons[i].y + 15, 20, BLACK);
            }
            break;

            case LOADING_SCREEN:
            DrawText("Carregando perguntas da IA...", screenWidth/2 - 200, screenHeight/2, 30, WHITE);
    
            break;

            case QUESTION_SCREEN:
                drawScore(5);
                DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
                drawQuestion(options, questions[_currentQuestion]);
                break;

            case ANSWER_SCREEN:
                drawScore(5);
                //adicionar função drawAnswer em outro arquivo
                DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
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
                drawScore(5);
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

    if (_state == LOADING_SCREEN) {
        pthread_join(loaderThread, NULL);
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

void *loadQuestionsThread(void *arg) {
    const char *topic = (const char *)arg;
    const char (*themes)[100] = getThemesOfTopic(topic);

    for (int j = 0; j < 5; j++) {
        questions[j] = addQuestion(topic, themes[j]);
    }

    _loadingFinished = true;
    return NULL;
}
