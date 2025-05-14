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
#include "string.h"
#include "board.h"


typedef enum {
    ROLL_DICE,
    ANSWER_QUESTION,
    SHOW_FEEDBACK
} TurnPhase;

int playerPositions[2] = {0, 0};
int currentPlayer = 0; // 0 ou 1
TurnPhase phase = ROLL_DICE;
int diceResult = 0;
bool questionAnsweredCorrectly = false;


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


static float timeRemaining = 0;
static bool timerActive = false;
static bool timeOut = false;

bool isTimerRunning() {  // Padronizando o nome (remova qualquer isTimeRunning)
    return timerActive;
}
void startTimer(float duration) {
    timeRemaining = duration;
    timerActive = true;
    timeOut = false;
}

void stopTimer() {
    timerActive = false;
}

void updateTimer() {
    if (timerActive) {
        timeRemaining -= GetFrameTime();
        if (timeRemaining <= 0) {
            timeRemaining = 0;
            timerActive = false;
            timeOut = true;
        }
    }
}

bool isTimeOut() {
    return timeOut;
}

float getRemainingTime() {
    return timeRemaining;
}

void resetTimer() {
    timeRemaining = 0;
    timerActive = false;
    timeOut = false;
}

void checkIfAnswerIsRight(Option *options, Question question);
void *loadQuestionsThread(void *arg);
const char* getTileTopic(char *topic);

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Quiz Game");
    SearchAndSetResourceDir("assets");
    lerJsonDeFormatacao("exemplo_questao.json");

    createBoard();
    
    Rectangle topicButtons[4];
    for (int i = 0; i < 4; i++) {
        topicButtons[i] = (Rectangle){ screenWidth/2 - 200, 150 + i*100, 400, 60 };
    }

    Rectangle mainMenuButtons[5];
    const char *mainMenuLabels[] = {
        "Modo Normal",
        "Modo Tabuleiro",
        "Hall da Fama",
        "Créditos",
		"Sair do Jogo"
    };
    for (int i = 0; i < 5; i++) {
        mainMenuButtons[i] = (Rectangle){ screenWidth/2 - 200, 200 + i*100, 400, 60 };
    }

    const char* tileLabels[3] = {
        "Inicio",
        "Casa AED",
        "Casa INFRA SO"
    };
    Rectangle nextQuestionButton = {screenWidth/4, screenHeight-100, 480, 320};

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
    Rectangle retornarButton = { 20, 20, 150, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) { // Botão de voltar com reset
        updateTimer();

        updateTimer();

        if (_menuOption == QUIZ_MODE && _quizScreen == QUESTION_SCREEN && !isTimerRunning()) {
            startTimer(15.0f); // 15 segundos para o modo quiz
        }
        
        if (isTimeOut()) {
            if (_menuOption == QUIZ_MODE) {
                _quizScreen = ANSWER_SCREEN;
            }
            resetTimer();
        }
        
    
        if (_menuOption != MAIN_MENU && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, retornarButton)) {
                _menuOption = MAIN_MENU;
                resetTimer();

                // Resetar quiz
                _quizScreen = TOPIC_SELECTION_SCREEN;
                _currentQuestion = 0;
                resetScore();
                _gotItRight = false;
                _loadingFinished = false;

                // Resetar tabuleiro
                freeBoard();
                createBoard();
            }
        }


        //UPTADE

        if (_menuOption == MAIN_MENU && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 5; i++) {
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
                            _menuOption = CREDITS;
                            break;
						case 4:
							CloseWindow();
							exit(0);
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
                Vector2 mouse = GetMousePosition();
                for (int i = 0; i < 4; i++) {
                    if (CheckCollisionPointRec(mouse, options[i].rect)) {
                        checkIfAnswerIsRight(options, questions[_currentQuestion]);
                        break;
                    }
                }
            }


            else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
                if (_quizScreen == FINAL_SCORE_SCREEN) { //RESETAR!!
                    _quizScreen = TOPIC_SELECTION_SCREEN;
                    _menuOption = MAIN_MENU;
                    saveScore("Nataniel");
                } else if (_quizScreen == ANSWER_SCREEN) {
                    _currentQuestion++;
                    isTimeOut();
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
            updateBoard();
        }

        BeginDrawing();

        ClearBackground((Color){ 135, 206, 235, 255 });


        switch (_menuOption)
{
    case MAIN_MENU:
        DrawText("MENU PRINCIPAL", screenWidth/2 - MeasureText("MENU PRINCIPAL", 40)/2, 80, 40, YELLOW);
        for (int i = 0; i < 5; i++) {
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
                DrawRectangleRec(retornarButton, DARKGRAY);
                DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
                break;

            case LOADING_SCREEN:
                DrawText("Carregando perguntas da IA...", screenWidth/2 - 200, screenHeight/2, 30, WHITE);
                DrawRectangleRec(retornarButton, DARKGRAY);
                DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
                break;

                case QUESTION_SCREEN:
    drawScore(5);
    DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 200, 30, 20, LIGHTGRAY);
    drawQuestion(options, questions[_currentQuestion]);

    // Timer - versão atualizada
    DrawRectangle(10, 10, 150, 40, (Color){0, 0, 0, 200});
    DrawText(TextFormat("Tempo: %.1f", getRemainingTime()), 20, 20, 30, 
           getRemainingTime() < 5.0f ? RED : WHITE);
    
    DrawRectangleRec(retornarButton, DARKGRAY);
    DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
    break;
    DrawText(TextFormat("Tempo: %.1f", getRemainingTime()), 40, 40, 30, 
           getRemainingTime() < 5.0f ? RED : WHITE);
    drawScore(5);

    DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 200, 30, 20, LIGHTGRAY);

    drawQuestion(options, questions[_currentQuestion]);

    DrawRectangle(10, 10, 150, 40, (Color){0, 0, 0, 200});
    DrawText(TextFormat("Tempo: %.1f", getRemainingTime()), 40, 40, 30, // pra mudar o posicionamento do timer
    getRemainingTime() < 5.0f ? RED : WHITE);
    
    DrawRectangleRec(retornarButton, DARKGRAY);
    DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
    break;

                case ANSWER_SCREEN:
                drawScore(5);
                DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
                DrawText("Gabarito:", 50, 100, 28, YELLOW);
                DrawText(questions[_currentQuestion].answer, 200, 100, 28, WHITE);
                
                if (isTimeOut()) {  // Usando a função do timer.h
                    DrawText("TEMPO ESGOTADO!", 50, 200, 40, ORANGE);
                    DrawText("ERROU!", 50, 250, 40, RED);
                } else {
                    DrawText(_gotItRight ? "ACERTOU!" : "ERROU!", 50, 200, 40, _gotItRight ? GREEN : RED);
                }
                
                DrawRectangleRec(nextQuestionButton, RED);
                DrawText("CONTINUAR", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                DrawRectangleRec(retornarButton, DARKGRAY);
                DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
                break;

            case FINAL_SCORE_SCREEN:
                drawScore(5);
                DrawText("QUIZ CONCLUÍDO!", screenWidth/2 - MeasureText("QUIZ CONCLUÍDO!", 40)/2, 100, 40, YELLOW);
                DrawText(TextFormat("Pontuação Final: %d/5", getScore()),
                         screenWidth/2 - MeasureText("Pontuação Final: 0/5", 30)/2, 200, 30, WHITE);
                DrawRectangleRec(nextQuestionButton, GREEN);
                DrawText("JOGAR NOVAMENTE", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                DrawRectangleRec(retornarButton, DARKGRAY);
                DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
                break;
        }
        break;

    case HALL_OF_FAME:
        drawHallOfFame();
        DrawRectangleRec(retornarButton, DARKGRAY);
        DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
        break;

    case TABULEIRO_MODE:
        drawBoard();
        DrawRectangleRec(retornarButton, DARKGRAY);
        DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
        break;

    case CREDITS:
        ClearBackground(BLACK);

        DrawText("CRÉDITOS", screenWidth/2 - MeasureText("CRÉDITOS", 40)/2, 80, 40, WHITE);

        int baseY = 200;
        int gap = 60;

        DrawText("Antônio Laprovitera - Programador", screenWidth/2 - 200, baseY + 0*gap, 24, GREEN);
        DrawText("Pedro Gusmão - Programador",        screenWidth/2 - 200, baseY + 1*gap, 24, GREEN);
        DrawText("Fernando Augusto - Programador",    screenWidth/2 - 200, baseY + 2*gap, 24, GREEN);
        DrawText("Felipe Andrade - Programador",      screenWidth/2 - 200, baseY + 3*gap, 24, GREEN);

        DrawRectangleRec(retornarButton, DARKGRAY);
        DrawText("Voltar", retornarButton.x + 20, retornarButton.y + 10, 20, WHITE);
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
    stopTimer();  // Usando a função do timer
    
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