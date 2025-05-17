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
#include "board.h"
#include "main_menu.h"
#include "player_names.h"
#include <subthemes.h>

#define MENU_OPTIONS_FONT_SIZE 64

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

Font titleFont = {0};
Font mainMenuFont = {0};
const char *titulo = "MIND RUNNER";

void checkIfAnswerIsRight(Option *options, Question question);
void *loadQuestionsThread(void *arg);
const char* getTileTopic(char *topic);

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Quiz Game");
    SearchAndSetResourceDir("assets");
    lerJsonDeFormatacao("exemplo_questao.json");

    titleFont = LoadFont("fonts/VT323-Regular.ttf");
    mainMenuFont = LoadFont("fonts/setback.png");

    //createBoard();
    
    Rectangle topicButtons[4];
    for (int i = 0; i < 4; i++) {
        topicButtons[i] = (Rectangle){ screenWidth/2 - 200, 150 + i*100, 400, 60 };
    }

    Rectangle mainMenuButtons[5];
    const char *mainMenuLabels[] = {
        "Modo Normal",
        "Modo Tabuleiro",
        "Hall da Fama",
        "Creditos",
		"Sair do Jogo"
    };

    int menuOptionsSpacing = 150;
    int menuOptionStartY = 250;

    for (int i = 0; i < 5; i++) {
        const char *upperText = TextToUpper(mainMenuLabels[i]);
        Vector2 textSize = MeasureTextEx(mainMenuFont, upperText, MENU_OPTIONS_FONT_SIZE, 2);

        // Centraliza o botão em relação ao texto já centralizado
        float rectX = screenWidth/2 - textSize.x/2 - 20; // 20 de padding horizontal
        float rectY = menuOptionStartY + i * menuOptionsSpacing;
        float rectWidth = textSize.x + 40; // 20 de padding dos dois lados
        float rectHeight = textSize.y + 20; // padding vertical

        mainMenuButtons[i] = (Rectangle){ rectX, rectY, rectWidth, rectHeight };
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
    Rectangle returnButton= { 20, 20, 150, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) { // Botão de voltar com reset
        if (_menuOption != MAIN_MENU && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, returnButton)) {
                switch (_menuOption) {
                    case QUIZ_MODE:
                        _menuOption = MAIN_MENU;
                        _quizScreen = TOPIC_SELECTION_SCREEN;
                        _currentQuestion = 0;
                        resetScore();
                        _gotItRight = false;
                        _loadingFinished = false;
                        break;

                    case TABULEIRO_MODE:
                        _menuOption = MAIN_MENU;
                        freeBoard();
                        createBoard();
                        break;

                    case HALL_OF_FAME:
                    case CREDITS:
                    case FINAL_MENU_SCREEN:
                        _menuOption = MAIN_MENU;
                        break;

                    default:
                        break;
                }
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
							showPlayerNameInputScreen();
                            createBoard();
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
            case MAIN_MENU: {
                // Fundo animado com variação suave de cores
                float t = GetTime();
                int r = 30 + 20 * sinf(t);
                int g = 50 + 20 * sinf(t + 1.0f);
                int b = 70 + 20 * sinf(t + 2.0f);
                ClearBackground((Color){ r, g, b, 255 });

                // Bolhas subindo
                for (int i = 0; i < 50; i++) {
                    float y = (float)(screenHeight - fmod(GetTime() * 50 + i * 40, screenHeight));
                    float x = 100 + (i * 30) % screenWidth;
                    DrawCircle(x, y, 5, Fade(WHITE, 0.3f));
                }

                Vector2 textSizeTitulo = MeasureTextEx(titleFont, titulo, 80, 2);
                DrawTextEx(titleFont, titulo, (Vector2){screenWidth/2 - textSizeTitulo.x/2, 80}, 80, 2, YELLOW);

                for (int i = 0; i < 5; i++) {
                    if (CheckCollisionPointRec(GetMousePosition(), mainMenuButtons[i]))
                    {
                        DrawRectangleRounded(mainMenuButtons[i], 0.5f, 10, DARKGRAY);
                    }
                    
                    const char *upperText = TextToUpper(mainMenuLabels[i]);
                    Vector2 textSize = MeasureTextEx(mainMenuFont, upperText, MENU_OPTIONS_FONT_SIZE, 2);
                    DrawTextEx(mainMenuFont, upperText,
                        (Vector2){
                            mainMenuButtons[i].x + (mainMenuButtons[i].width - textSize.x)/2,
                            mainMenuButtons[i].y + (mainMenuButtons[i].height - textSize.y)/2
                        },
                        MENU_OPTIONS_FONT_SIZE, 2, WHITE);
                }
                break;
            }


    case QUIZ_MODE:
        switch (_quizScreen) {
            case TOPIC_SELECTION_SCREEN:
                DrawText("Selecione um Tópico", screenWidth/2 - MeasureText("Selecione um Tópico", 30)/2, 50, 30, YELLOW);
                for (int i = 0; i < 4; i++) {
                    DrawRectangleRec(topicButtons[i], GRAY);
                    DrawText(topics[i], topicButtons[i].x + 10, topicButtons[i].y + 15, 20, BLACK);
                }
                DrawRectangleRec(returnButton, DARKGRAY);
                DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
                break;

            case LOADING_SCREEN:
                DrawText("Carregando perguntas da IA...", screenWidth/2 - 200, screenHeight/2, 30, WHITE);
                DrawRectangleRec(returnButton, DARKGRAY);
                DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
                break;

            case QUESTION_SCREEN:
                drawScore(5);
                DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
                drawQuestion(options, questions[_currentQuestion]);
                DrawRectangleRec(returnButton, DARKGRAY);
                DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
                break;

            case ANSWER_SCREEN:
                drawScore(5);
                DrawText(TextFormat("Questão %d/%d", _currentQuestion + 1, 5), screenWidth - 150, 30, 20, LIGHTGRAY);
                DrawText("Gabarito:", 50, 100, 28, YELLOW);
                DrawText(questions[_currentQuestion].answer, 200, 100, 28, WHITE);
                DrawText(_gotItRight ? "ACERTOU!" : "ERROU!", 50, 200, 40, _gotItRight ? GREEN : RED);
                DrawRectangleRec(nextQuestionButton, RED);
                DrawText("CONTINUAR", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                DrawRectangleRec(returnButton, DARKGRAY);
                DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
                break;

            case FINAL_SCORE_SCREEN:
                drawScore(5);
                DrawText("QUIZ CONCLUÍDO!", screenWidth/2 - MeasureText("QUIZ CONCLUÍDO!", 40)/2, 100, 40, YELLOW);
                DrawText(TextFormat("Pontuação Final: %d/5", getScore()),
                         screenWidth/2 - MeasureText("Pontuação Final: 0/5", 30)/2, 200, 30, WHITE);
                DrawRectangleRec(nextQuestionButton, GREEN);
                DrawText("JOGAR NOVAMENTE", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
                DrawRectangleRec(returnButton, DARKGRAY);
                DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
                break;
        }
        break;

    case HALL_OF_FAME:
        drawHallOfFame();
        DrawRectangleRec(returnButton, DARKGRAY);
        DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
        break;

    case TABULEIRO_MODE:
        drawBoard();
        DrawRectangleRec(returnButton, DARKGRAY);
        DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
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

        DrawRectangleRec(returnButton, DARKGRAY);
        DrawText("Voltar", returnButton.x + 20, returnButton.y + 10, 20, WHITE);
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

void *loadQuestionsThread(void *arg) {
    const char *topic = (const char *)arg;
    const char (*themes)[100] = getThemesOfTopic(topic);
    
    for (int j = 0; j < 5; j++) {
        questions[j] = addQuestion(topic, topicToTheme(themes[j]));
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