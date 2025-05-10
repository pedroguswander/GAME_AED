#include "raylib.h"
#include "raymath.h"
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
#include "hall.h"
#include "time.h"
#include "string.h"

typedef enum {
    TOPIC_SELECTION_SCREEN =0,
    LOADING_SCREEN = 1,
    QUESTION_SCREEN = 2,
    ANSWER_SCREEN = 3,
    FINAL_SCORE_SCREEN = 4,
    TABULEIRO_QUESTION_SCREEN = 5 
} QuizScreen;

typedef enum {
    MAIN_MENU=0,
    QUIZ_MODE=1,
    TABULEIRO_MODE=2,
    HALL_OF_FAME=3,
    CREDITS=4
} MenuOption;

typedef enum {
    QUESTION=0,
    BOSS=1,
    SPECIAL_EVENT=2,
} TileType;

typedef struct {
    Vector2 position;
    Vector2 prevPosition;
    int currentTile;
    int prevTile;
} Player;

typedef struct {
    Vector2 position;
    TileType type;
    char topic[100];
} Tile;

#define TILE_DISTANCE 150

int _rool = 0;
int acertou;
Question tileQuestion = {0};

Tile _tiles[3] = {0};
MenuOption _menuOption = MAIN_MENU;
QuizScreen _quizScreen = 0;
char *_current_topic = "Algoritmos e Estruturas de Dados"; //escolha o tópico
bool _gotItRight = false;
bool _quizLoaded = false;
int _currentQuestion = 0;
Question questions[5];
pthread_t loaderThread;
bool _loadingFinished = false;
const char *topics[] = {
    "Algoritmos e Estruturas de Dados",
    "INFRA_SO",
    "POO",
    "Historia do Brasil"
    };


void checkIfAnswerIsRight(Option *options, Question question);
void *loadQuestionsThread(void *arg);
void loadQuestionThread(void *arg);
const char* getTileTopic(char *topic);

int main() {

    const int screenWidth = 1280;
    const int screenHeight = 800;

    
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

    const char* tileLabels[3] = {
        "Inicio",
        "Casa AED",
        "Casa INFRA SO"
    };

    for (int i = 0; i < 3; i ++)
    {
        _tiles[i].position = (Vector2) {200+(TILE_DISTANCE*i), 600};
        _tiles[i].type = QUESTION;
        strcpy(_tiles[i].topic, tileLabels[i]); 
    }

    Rectangle tileRects[3] = {
        {_tiles[0].position.x - 50, _tiles[0].position.y - 50, 100, 100},
        {_tiles[1].position.x - 50, _tiles[1].position.y - 50, 100, 100},
        {_tiles[2].position.x - 50, _tiles[2].position.y - 50, 100, 100}
    };

    Player player = {
        _tiles->position,
        (Vector2) {0,0},
        0,
        0
    };

    int targetTile = 0;
    bool isMoving = false;

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

    srand(time(NULL)); 

    while (!WindowShouldClose()) {
        //UPTADE

        if (_menuOption == MAIN_MENU && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mouse, mainMenuButtons[i])) {
                    switch(i) {
                        case 0: // Modo Normal
                            _menuOption = QUIZ_MODE;
                            break;
                        case 1: // Modo Tabuleiro
                            _menuOption = TABULEIRO_MODE;
                            break;
                        case 2: // Hall da Fama
                            _menuOption = HALL_OF_FAME;
                            // Implementar depois
                            break;
                        case 3: // Créditos
                            // Implementar depois
                            break;
                    }
                }
            }
        }

        if (_menuOption == HALL_OF_FAME) {
            uptadeHallOfFame();
        }
        
        if (_menuOption == QUIZ_MODE) {
            if (_quizScreen == TOPIC_SELECTION_SCREEN && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                for (int i = 0; i < 4; i++) { //uptadeTopics
                    if (CheckCollisionPointRec(GetMousePosition(), topicButtons[i])) {
                        _current_topic = (char *)topics[i];
                        resetScore();
                        _currentQuestion = 0;
                        _loadingFinished = false;
                        _quizScreen = LOADING_SCREEN;
                        
                        // Cria uma thread para carregar perguntas
                        pthread_create(&loaderThread, NULL, loadQuestionsThread, (void *)_current_topic);
                        break;
                    }
                }
            }

            if (_quizScreen == LOADING_SCREEN)
            {
                if (_loadingFinished) {
                    _quizScreen = QUESTION_SCREEN;
                }
            }

        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _quizScreen == QUESTION_SCREEN) {
            checkIfAnswerIsRight(options, questions[_currentQuestion]);
        }

        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
            if (_quizScreen == FINAL_SCORE_SCREEN) { //RESETAR!!
                _quizScreen = TOPIC_SELECTION_SCREEN;
                _menuOption = MAIN_MENU;
                saveScore("Nataniel");
            } else if (_quizScreen == ANSWER_SCREEN) {
                _currentQuestion++;
                if (_currentQuestion >= 5) {
                    _quizScreen = FINAL_SCORE_SCREEN;
                } else {
                    _quizScreen = QUESTION_SCREEN;
                }
                _gotItRight = false;
            }
        }
    }

    if (_menuOption == TABULEIRO_MODE)
        {
        if (!isMoving && IsKeyPressed(KEY_SPACE)) { //lidarcomJogada()
                _rool = rand() % 3 + 1; // 1 a 3
                targetTile = player.currentTile + _rool;
                if (targetTile > 2) targetTile = 2;
                player.prevPosition = player.position;
                player.prevTile = player.currentTile;
                isMoving = true;
            }

            if (isMoving) {
                Vector2 targetPos = _tiles[targetTile].position;
                Vector2 direction = Vector2Subtract(targetPos, player.position);
                float distance = Vector2Length(direction);
                if (distance < 2.0f) {
                    player.position = targetPos;
                    player.currentTile = targetTile;
                    isMoving = false;
                    //new question = Addquestion
                    pthread_create(&loaderThread, NULL, loadQuestionThread, 
                    (void *)getTileTopic(_tiles[player.currentTile].topic));

                    _quizScreen = TABULEIRO_QUESTION_SCREEN;
   

                    if (!_gotItRight)
                    {
                        targetPos = player.prevPosition;
                        targetTile = player.prevTile;
                        isMoving = true;
                    }

                    /*acertou = rand() % 2;
                    if (!acertou)
                    {
                        targetPos = player.prevPosition;
                        targetTile = player.prevTile;
                        isMoving = true;
                    }*/

                } else {
                    direction = Vector2Normalize(direction);
                    player.position.x += direction.x * 5;
                    player.position.y += direction.y * 5;
                }
            }
    }

    if (_quizScreen == TABULEIRO_QUESTION_SCREEN && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    checkIfAnswerIsRight(options, tileQuestion);
    
    if (_gotItRight) {
        _quizScreen = -1; // nenhum screen ativo, volta ao tabuleiro
    } else {
        player.position = player.prevPosition;
        player.currentTile = player.prevTile;
        _quizScreen = -1;
    }
}


        BeginDrawing();

        ClearBackground(BLACK);

        switch (_menuOption)
        {
            case MAIN_MENU:
                DrawText("MENU PRINCIPAL", screenWidth/2 - MeasureText("MENU PRINCIPAL", 40)/2, 80, 40, YELLOW);
                for (int i = 0; i < 4; i++) {
                    DrawRectangleRec(mainMenuButtons[i], DARKGRAY);
                    DrawText(mainMenuLabels[i], mainMenuButtons[i].x + 20, mainMenuButtons[i].y + 15, 24, WHITE);
                }
                break;

                case QUIZ_MODE:
                    switch (_quizScreen) {
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
                            DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
                            DrawText("Gabarito:", 50, 100, 28, YELLOW);
                            DrawText(questions[_currentQuestion].answer, 200, 100, 28, WHITE);
                            DrawText(_gotItRight ? "ACERTOU!" : "ERROU!", 50, 200, 40, _gotItRight ? GREEN : RED);
                            DrawRectangleRec(nextQuestionButton, RED);
                            DrawText("CONTINUAR", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                            break;
                
                        case FINAL_SCORE_SCREEN:
                            drawScore(5);
                            DrawText("QUIZ CONCLUÍDO!", screenWidth/2 - MeasureText("QUIZ CONCLUÍDO!", 40)/2, 100, 40, YELLOW);
                            DrawText(TextFormat("Pontuação Final: %d/5", getScore()), 
                                    screenWidth/2 - MeasureText("Pontuação Final: 0/5", 30)/2, 200, 30, WHITE);
                            DrawRectangleRec(nextQuestionButton, GREEN);
                            DrawText("JOGAR NOVAMENTE", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                            break;
                    }
                    break;

        case HALL_OF_FAME:
            drawHallOfFame();
            break;
        
        case TABULEIRO_MODE:

            if (_quizScreen == TABULEIRO_QUESTION_SCREEN) {
             drawQuestion(options, tileQuestion);
            }

            else {
            DrawText("MODO TABULEIRO - Pressione SPACE para rolar o dado", 20, 20, 20, DARKGRAY);
            DrawText(TextFormat("DADO %d", _rool), 20, 40, 20, DARKGRAY);
            DrawText(TextFormat("%d", acertou), 20, 60, 20, DARKGRAY);

            // Desenhar casas
            for (int i = 0; i < 3; i++) {
                DrawRectangleRec(tileRects[i], LIGHTGRAY);
                DrawRectangleLinesEx(tileRects[i], 2, DARKGRAY);
                DrawText(tileLabels[i], tileRects[i].x + 10, tileRects[i].y + 40, 16, BLACK);
            }

            // Desenhar jogador
            DrawCircleV(player.position, 20, RED);
            }
            break;
        
        default:
            break;

    }

    EndDrawing();

    if (_quizScreen == LOADING_SCREEN) {
        pthread_join(loaderThread, NULL);
    }

}
    CloseWindow();
    return 0;

}

void checkIfAnswerIsRight(Option *options, Question question) {
    Vector2 mouse = GetMousePosition();
    _quizScreen = ANSWER_SCREEN;
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

void loadQuestionThread(void *arg)
{    
    const char *topic = (const char *)arg;
    const char (*themes)[100] = getThemesOfTopic(topic);

    //tema aleatorio

    tileQuestion = addQuestion(topic, themes[0]);


    _loadingFinished = true;
    return NULL;

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

const char* getTileTopic(char *topic)
{
    if (strcmp(topic, "Casa AED") == 0)
    {
        return topics[0];
    }
    else if (strcmp(topic, "Casa INFRA_SO") == 0)
    {
        return topics[1];
    }

    return NULL;
}