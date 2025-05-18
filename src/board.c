// board.c
#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "time.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "question.h"
#include "pthread.h"
#include "prompt.h"
#include "player_animation.h"
#include "dice_animation.h"
#include "rlgl.h"
#include "main_menu.h"
#include "player_names.h"
#include "end_screen.h"

#define BOARD_SIZE 15
#define PLAYER1_TEXT_SIZE 32
#define PLAYER1_TEXT_SCALE 3
#define MAX_PLAYERS 2

typedef enum {
    EVENT_NONE,
    EVENT_DISPLAY_TOPIC,
    EVENT_LOADING,
    EVENT_QUESTION,
} EventState;

Font fontPlayerName = {0};

Texture2D backgroundTexture;
BoardState _boardState = WAITING_TO_END;
EventState _eventState = EVENT_NONE;

Camera2D _boardCamera = {0};

Sound victoyTheme;
Tile *_tilesHEAD = NULL;
Tile *_tilesTAIL = NULL;

Player _players[MAX_PLAYERS];
Tile *tileBeforePlaying = NULL;
#define COLOR_P1 CLITERAL (Color) {22, 147, 12, 255}
#define COLOR_P2 CLITERAL (Color) {113, 27, 178, 255}
Color _playerColors[] = {COLOR_P1, COLOR_P2};

Rectangle _playerTextSrc = {0};
Rectangle _playerTextDest = {0};

bool _loadingFinishedBoard = false;
pthread_t _loadThread;

Question _questionTile = {0};
Option options[4];
char *labels[] = {"A", "B", "C", "D"};
Rectangle optionRects[4];
Rectangle nextQuestionButton = {1920/4, 1080-100, 480, 320};

char *_playersNames[] = {player1Name, player2Name};

int startY = 100;
int _dice;
int _targetTile = 0;
int _acertou = 0;
int _gotItRigth = 0;
int _currentPlayerIndex = 0;
float stepTimer = 0.0f;
float stepDelay = 0.5f; 
bool isWaiting = false;
float waitingToEndTimer = 0.0f;
float waitingToEndDuration = 0.5f;

static bool diceRolled = false;
static double diceRollTime = 0;

const Vector2 tilePositions[BOARD_SIZE] = {
    {400, 940},
    {200, 770},
    {600, 770},
    {1000, 770},
    {1400, 770},
    {1730, 490},
    {1400, 530},
    {1000, 530},
    {600, 530},
    {200, 530},
    {180, 290},
    {600, 300},
    {1000, 300},
    {1400, 300},
    {1500, 70},
};

const char* tileLabels[BOARD_SIZE] = {
    "Início", "Conhecimentos Gerais", "Filmes", "Músicas", "Matemática",
    "Boss Node AED", "Filmes", "Músicas", "Conhecimentos Gerais", "Filmes",
    "Boss Node AED", "Matemática", "Conhecimentos Gerais", "Músicas", "Final"
};

void resetBoard() {
    _boardState = WAITING_TO_END;
    _eventState = EVENT_NONE;
    _loadingFinishedBoard = false;
    _dice = 0;
    _targetTile = 0;
    _acertou = 0;
    _gotItRigth = 0;
    _currentPlayerIndex = 0;
    stepTimer = 0.0f;
    waitingToEndTimer = 0.0f;
    isWaiting = false;
    tileBeforePlaying = NULL;
    _playerColors[0] = COLOR_P1;
    _playerColors[1] = COLOR_P2;
    strcpy(_playersNames[0], player1Name);
    strcpy(_playersNames[1], player2Name);
}


void *loadQuestionThread(void *arg) {
    Tile *tile = (Tile *)arg;
    Theme temaEnum = topicToTheme(tile->topic);
    Question newQuestion = (temaEnum != THEME_COUNT) ? addQuestion(tile->topic, temaEnum) : (Question){0};

    // Libera a memória da questão anterior, se existir
    if (_questionTile.statement != NULL) {
        freeQuestion(_questionTile);
    }
    _questionTile = newQuestion;
    _loadingFinishedBoard = true;
    return NULL;
}

void createTile(TileType type, const char *topic, int tile) {
    Tile *new_tile = (Tile *) malloc(sizeof(Tile));
    if (new_tile) {
        new_tile->position = tilePositions[tile];
        new_tile->type = type;
        new_tile->rect = (Rectangle){new_tile->position.x - 50, new_tile->position.y - 50, 100, 100};
        strcpy(new_tile->topic, topic);
        new_tile->tile = tile;
        new_tile->next = NULL;
        new_tile->prev = NULL;

        if (_tilesHEAD == NULL) {
            _tilesHEAD = new_tile;
            _tilesTAIL = new_tile;
        } else {
            _tilesTAIL->next = new_tile;
            new_tile->prev = _tilesTAIL;
            _tilesTAIL = new_tile;
        }
    }
}

void createBoard() {
    resetBoard();

    InitAudioDevice();

    _boardState = WAITING_TO_END;
    _eventState = EVENT_NONE;
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;

    backgroundTexture = LoadTexture("background-board-mode-1.png");
    victoyTheme = LoadSound("music/videoplayback.wav");
    fontPlayerName = LoadFont("fonts/pixantiqua.png");

    InitPlayerAnimation();
    initDice();

    _playerTextSrc = (Rectangle){0, 0, PLAYER1_TEXT_SIZE, PLAYER1_TEXT_SIZE};

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40};
        options[i].rect = optionRects[i];
        strcpy(options[i].answer, labels[i]);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        TileType type = QUESTION; // Tipo padrão é QUESTION
        if (strcmp(tileLabels[i], "Boss Node AED") == 0) {
            // type = BOSS;
        }
        createTile(type, tileLabels[i], i);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        _players[i] = (Player){
            _tilesHEAD->position,
            (Vector2){0, 0},
            _tilesHEAD,
            NULL,
            NULL,
            NULL,
            i+1,
            false,
            _playerColors[i],
            CAN_PLAY,
            (Texture2D) {0},
            _playersNames[i],
        };

        setSpriteToIdle(&_players[i], i);
    }

    _boardCamera = (Camera2D) {
        (Vector2){GetScreenWidth()/2, GetScreenHeight()/2},
        _players->position,
        0,
        1
    };
}

Vector2 getPositionOfTile(int tile) {
    Tile *iter = _tilesHEAD;
    while (iter) {
        if (iter->tile == tile) return iter->position;
        iter = iter->next;
    }
    return (Vector2){0};
}

Tile *getTileByTile(int tile) {
    Tile *iter = _tilesHEAD;
    while (iter) {
        if (iter->tile == tile) return iter;
        iter = iter->next;
    }
    return NULL; 
}

int getIndexOfTile(Tile *tile) {
    if (!tile) return -1;
    Tile *current = _tilesHEAD;
    int index = 0;
    while (current) {
        if (current == tile) return index;
        current = current->next;
        index++;
    }
    return -1;
}

void updateBoard() {
    Player *player = &_players[_currentPlayerIndex];


        switch (_boardState) {

            case CAN_PLAY_TRANSITION:
                TraceLog(LOG_INFO, "%s", player1Name);
                TraceLog(LOG_INFO, "%s", player2Name);
                // Transição suave para o zoom de 3.0f
                _boardCamera.target = player->position;

                float zoomSpeed = 0.05f; // ajuste conforme necessário
                float targetZoom = 3.0f;

                // Interpola o zoom suavemente
                _boardCamera.zoom += (targetZoom - _boardCamera.zoom) * zoomSpeed;

                // Quando estiver suficientemente próximo do zoom alvo, muda para CAN_PLAY
                if (fabsf(_boardCamera.zoom - targetZoom) < 0.05f) {
                    _boardCamera.zoom = targetZoom; // garante valor exato
                    _boardState = CAN_PLAY;
                }   
                break;

            case CAN_PLAY:

                _boardCamera.target = player->position;

                _boardCamera.zoom = 3.0f;
                if (diceRolled) {
                    // Espera 0.8 segundos após o lançamento do dado antes de iniciar o movimento
                    if (GetTime() - diceRollTime >= 0.8) {
                        int index = getIndexOfTile(player->currentTile);
                        int destino = index + _dice;
                        if (destino >= BOARD_SIZE) destino = BOARD_SIZE - 1;
                        player->targetTile = getTileByTile(destino);
                        tileBeforePlaying = player->currentTile;

                        _boardState = MOVING;                        
                        diceRolled = false; // Reset para o próximo turno
                    }
                } else if (IsKeyPressed(KEY_SPACE)) {
                    _dice = rand() % 6 + 1;
                    setDiceResult(_dice);
                    diceRolled = true;
                    diceRollTime = GetTime(); // Marca o momento em que o dado foi lançado
                }
                else
                {
                    updateDice();
                }
            break;

        case MOVING:
            if (!movePlayer(player, true))
            {
                UpdatePlayerAnimation();
                setPlayerSpriteAnimation(player);
                break;
            }
            
            // Mantém o flip atual quando voltar para idle
            setSpriteToIdle(player, _currentPlayerIndex);
            _boardState = EVENT;
            _eventState = EVENT_DISPLAY_TOPIC;
            break;

        case MOVING_BACKWARDS:
            if (!movePlayer(player, false))
            {
                UpdatePlayerAnimation();
                setPlayerSpriteAnimation(player);
                break;
            } 

            setSpriteToIdle(player, _currentPlayerIndex);
            finalizeTurn();
            break;

        case EVENT:
            switch (_eventState) {
                case EVENT_DISPLAY_TOPIC:
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (player->currentTile->type == QUESTION) {
                            _loadingFinishedBoard = false;
                            pthread_create(&_loadThread, NULL, loadQuestionThread, player->currentTile);
                            _eventState = EVENT_LOADING;
                        } else if (player->currentTile->type == BOSS) { 
                            // // Tratamento básico inicial: apenas finalize o turno
                            // printf("Jogador caiu na Boss Node AED.\n");
                            // printf("Finalizando turno do Jogador %d.\n", _currentPlayerIndex + 1);
                            // finalizeTurn();
                        } else {
                            finalizeTurn();
                        }
                    }
                    break;

                case EVENT_LOADING:
                    if (_loadingFinishedBoard) {
                        pthread_join(_loadThread, NULL);
                        _eventState = EVENT_QUESTION;
                    }
                    break;

                case EVENT_QUESTION:
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 mouse = GetMousePosition();
                        for (int i = 0; i < 4; i++) {
                            if (CheckCollisionPointRec(mouse, options[i].rect)) {
                                _gotItRigth = strcmp(options[i].answer, _questionTile.answer) == 0;
                                _boardState = SHOW_ANSWER;
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
            break;

        case SHOW_ANSWER:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton)) {
                if (_gotItRigth) {
                    if (player->currentTile->tile >= BOARD_SIZE -1)
                        _boardState = WAITING_TO_END;
                    
                    else finalizeTurn();
                } else {
                    player->targetTile = tileBeforePlaying;
                    _boardState = MOVING_BACKWARDS;
                }
            }
            break;

        case WAITING_TO_END:
            waitingToEndTimer += GetFrameTime();
            if (waitingToEndTimer >= waitingToEndDuration) {
                waitingToEndTimer = 0.0f;
                _boardState = END;
            }
            break;

        case END:
            updateEndScreen();

            PlaySound(victoyTheme);
            break;
    }
}

void drawBoard() {
    switch (_boardState) {
        case EVENT:
            if (_eventState == EVENT_QUESTION) {

            }
            switch (_eventState) {
                case EVENT_DISPLAY_TOPIC:
                    DrawText("Carregando pergunta...", GetScreenWidth()/2 - 150, GetScreenHeight()/2+300, 30, BLACK);
                    DrawTexture(backgroundTexture, 0, 0, Fade(WHITE, 0.3f));


                    DrawText(_players[_currentPlayerIndex].currentTile->topic,
                            GetScreenWidth()/2 - 100, GetScreenHeight()/2 - 40,
                            40, BLACK);

                            
                    DrawText("Aperte clique para iniciar o quiz!!!", GetScreenWidth()/2 - 350,
                    GetScreenHeight()/2 - 300, 48, BLACK);
                    break;

                case EVENT_LOADING:
                    DrawText("Carregando pergunta...", GetScreenWidth()/2 - 150, GetScreenHeight()/2+300, 30, BLACK);
                    DrawTexture(backgroundTexture, 0, 0, Fade(WHITE, 0.3f));
                    DrawText(_players[_currentPlayerIndex].currentTile->topic,
                            GetScreenWidth()/2 - 100, GetScreenHeight()/2 - 40,
                            40, BLACK);

                    DrawText("Aperte clique para iniciar o quiz!!!", GetScreenWidth()/2 - 350,
                    GetScreenHeight()/2 - 300, 48, BLACK);
                    break;

                case EVENT_QUESTION:
                    drawQuestion(_questionTile);
                    DrawTextEx(fontPlayerName , TextFormat("%s", _players[_currentPlayerIndex].name),
                     (Vector2) {540, 900}, 32, 2, _playerColors[_currentPlayerIndex]);
                    break;

                default:
                    break;
            }

            break;

        case SHOW_ANSWER:
            DrawText("Gabarito:", 50, 100, 28, YELLOW);
            DrawText(_questionTile.answer, 200, 100, 28, WHITE);
            DrawText(_gotItRigth ? "ACERTOU!" : "ERROU!", 50, 200, 40, _gotItRigth ? GREEN : RED);
            DrawRectangleRec(nextQuestionButton, RED);
            DrawText("CONTINUAR", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
            break;

        case END:
            //DrawRectangleRec((Rectangle){0, 0, 1920, 1080}, Fade(PINK, 0.3f));
            //DrawTextEx(fontPlayerName, TextFormat("%s venceu!!", _players[_currentPlayerIndex].name),
            //(Vector2) {1920/2, 1080/2}, 20, 2, _players[_currentPlayerIndex].color);
            drawEndScreen(_players[_currentPlayerIndex]);
            break;

        
        case CAN_PLAY_TRANSITION:
            BeginMode2D(_boardCamera);

                DrawTexture(backgroundTexture, 0, 0, WHITE);
                DrawCircle(_boardCamera.target.x, _boardCamera.target.y, 10, RED);
                drawPlayer(&_players[_currentPlayerIndex]);

            EndMode2D();
            break;

        case CAN_PLAY:
            BeginMode2D(_boardCamera);

                DrawTexture(backgroundTexture, 0, 0, WHITE);
                DrawCircle(_boardCamera.target.x, _boardCamera.target.y, 10, RED);
                drawPlayer(&_players[_currentPlayerIndex]);

            EndMode2D();

            drawDice();
            DrawTextEx(fontPlayerName, _players[_currentPlayerIndex].name, 
                (Vector2){ 595, 509 }, 20, 2, _players[_currentPlayerIndex].color);
            DrawText(" - PRESSIONE SPACE PARA RODAR O DADO ", 595 + MeasureText(_players[_currentPlayerIndex].name, 20), 509, 20, BLACK);

            break;

        default:
            DrawTexture(backgroundTexture, 0, 0, WHITE);
            const char* msg = "MODO TABULEIRO - Pressione SPACE para rolar o dado";
            DrawText(msg, GetScreenWidth()/2 - MeasureText(msg, 20)/2, 20, 20, BLACK);
            DrawText(TextFormat("DADO %d", _dice), 961, 58, 20, DARKGRAY);
            DrawTextEx(fontPlayerName, "VEZ DE ", (Vector2) {540, 900}, 20, 2, BLACK);
            DrawTextEx(fontPlayerName, _players[_currentPlayerIndex].name,
                (Vector2){ 540 + MeasureText("VEZ DE ", 20), 900 }, 20, 2, _players[_currentPlayerIndex].color);

            for (int i = 0; i < MAX_PLAYERS; i++) {
                Player *p = &_players[i];
                drawPlayer(p);
            }

            if (_tilesHEAD) {
                Tile *current = _tilesHEAD;
                while (current) {
                    DrawRectangleRec(current->rect, LIGHTGRAY);
                    DrawRectangleLinesEx(current->rect, 2, DARKGRAY);
                    DrawText(current->topic, current->rect.x + 10, current->rect.y + 40, 16, BLACK);
                    current = current->next;
                }
            }

            break;
    }
}

void finalizeTurn() {
    tileBeforePlaying = NULL;
    _currentPlayerIndex = (_currentPlayerIndex + 1) % MAX_PLAYERS;
    _boardState = CAN_PLAY_TRANSITION;
}

bool movePlayer(Player *player, bool forward) {
    if (player->currentTile == NULL || player->targetTile == NULL) return true;

    // Se não tiver próximo tile definido, defina-o agora
    if (player->nextTile == NULL) {
        player->nextTile = forward ? player->currentTile->next : player->currentTile->prev;
        if (player->nextTile == NULL) return true;
    }

    Vector2 targetPos = player->nextTile->position;
    Vector2 direction = Vector2Subtract(targetPos, player->position);
    float distance = Vector2Length(direction);

    if (distance < 10.0f) {
        player->position = targetPos;
        player->prevTile = player->currentTile;
        player->currentTile = player->nextTile;

        if (player->currentTile == player->targetTile) {
            player->nextTile = NULL;
            return true;
        }

        player->nextTile = forward ? player->currentTile->next : player->currentTile->prev;
        if (player->nextTile == NULL) return true;
    } else {
        direction = Vector2Normalize(direction);
        Vector2 movement = Vector2Scale(direction, 250.0f * GetFrameTime());
        player->position = Vector2Add(player->position, movement);
    }

    return false;
}

void drawPlayer(Player *p)
{
    Vector2 playerCenter = {
        p->position.x + p->number * 40 + (_playerTextSrc.width * PLAYER1_TEXT_SCALE)/2,
        p->position.y + (_playerTextSrc.height * PLAYER1_TEXT_SCALE)/2
    };
    
    // Draw player texture
    _playerTextDest = (Rectangle){
        p->position.x + p->number * 40, 
        p->position.y,
        _playerTextSrc.width * PLAYER1_TEXT_SCALE, 
        _playerTextSrc.height * PLAYER1_TEXT_SCALE
    };
    DrawTexturePro(p->sprite, _playerTextSrc, _playerTextDest, (Vector2){0, 0}, 0, WHITE);
    
    const char* playerLabel = TextFormat("P%d: %s", p->number, p->name);
    int labelWidth = MeasureText(playerLabel, 20);
    DrawTextEx(fontPlayerName,
            playerLabel,
            (Vector2){playerCenter.x - labelWidth/2, p->position.y - 20},          
            10,
            2,
            p->color);
}

void freeBoard() {
    // Libera texturas e sons
    UnloadTexture(backgroundTexture);
    UnloadSound(victoyTheme);
    UnloadPlayerAnimation();

    // Finaliza a thread se estiver ativa
    if (!_loadingFinishedBoard) {
        pthread_cancel(_loadThread); // força o encerramento
        pthread_join(_loadThread, NULL);
    }

    // Libera lista de tiles
    Tile *current = _tilesHEAD;
    while (current) {
        Tile *next = current->next;
        free(current);
        current = next;
    }

    _tilesHEAD = NULL;
    _tilesTAIL = NULL;


    // Reseta questão atual
    memset(&_questionTile, 0, sizeof(Question));
}