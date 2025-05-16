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

Texture2D backgroundTexture;
BoardState _boardState = CAN_PLAY;
EventState _eventState = EVENT_NONE;

Sound victoyTheme;
Tile *_tilesHEAD = NULL;
Tile *_tilesTAIL = NULL;

Player _players[MAX_PLAYERS];

Texture2D _playerText = {0};
Rectangle _playerTextSrc = {0};
Rectangle _playerTextDest = {0};

bool _loadingFinishedBoard = false;
pthread_t _loadThread;

Question _questionTile = {0};
Option options[4];
char *labels[] = {"A", "B", "C", "D"};
Rectangle optionRects[4];
Rectangle nextQuestionButton = {1920/4, 1080-100, 480, 320};

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
Tile *tileBeforePlaying = NULL;
Color _playerColor = {0};

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
    "Conhecimentos Gerais", "Filmes", "Músicas", "Conhecimentos Gerais", "Filmes",
    "Conhecimentos Gerais", "Matemática", "Conhecimentos Gerais", "Músicas", "Filmes",
};

void resetBoard() {
    _boardState = CAN_PLAY;
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
    _playerColor = (Color){0};
}


void *loadQuestionThread(void *arg) {
    Tile *tile = (Tile *)arg;
    const char (*themes)[100] = getThemesOfTopic(tile->topic);
    _questionTile = themes ? addQuestion(tile->topic, themes[0]) : (Question){0};
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
    _boardState = CAN_PLAY;
    _eventState = EVENT_NONE;
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;
    backgroundTexture = LoadTexture("background-board-mode-1.png");
    victoyTheme = LoadSound("music/videoplayback.wav");

    InitPlayerAnimation();

    _playerTextSrc = (Rectangle){0, 0, PLAYER1_TEXT_SIZE, PLAYER1_TEXT_SIZE};

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40};
        options[i].rect = optionRects[i];
        strcpy(options[i].answer, labels[i]);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        createTile(QUESTION, tileLabels[i], i);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        _playerColor = (i == 0) ? RED : BLUE;

        _players[i] = (Player){
            _tilesHEAD->position,
            (Vector2){0, 0},
            _tilesHEAD,
            NULL,
            NULL,
            NULL,
            i+1,
            false,
            _playerColor,
            CAN_PLAY,
            playerIdleSprite,
        };
    }
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
        case CAN_PLAY:
            if (IsKeyPressed(KEY_SPACE)) {
                _dice = rand() % 3 + 1;
                int index = getIndexOfTile(player->currentTile);
                int destino = index + _dice;
                if (destino >= BOARD_SIZE) destino = BOARD_SIZE - 1;
                player->targetTile = getTileByTile(destino);
                tileBeforePlaying = player->currentTile;
                _boardState = MOVING;
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
            _playerTextSrc.width = player->flipHorizontal ? -abs(_playerTextSrc.width) : abs(_playerTextSrc.width);
            player->sprite = playerIdleSprite;
            _boardState = EVENT;
            _eventState = EVENT_DISPLAY_TOPIC;
            break;

        case MOVING_BACKWARDS:
            if (!movePlayer(player, false)) break;
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
                    finalizeTurn();
                    if (player->currentTile->tile >= BOARD_SIZE -1)
                        _boardState = WAITING_TO_END;
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
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {0, 0, GetScreenWidth(), GetScreenHeight()}))
            {

            }
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
                    drawQuestion(options, _questionTile);
                    DrawText(TextFormat("P%d", _currentPlayerIndex + 1), 540, 900, 32, _playerColor);
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
            DrawRectangleRec((Rectangle){0, 0, 1920, 1080}, BLUE);
            DrawText(TextFormat("Jogador: %d venceu!!", _currentPlayerIndex + 1),
                     1920/2, 1080/2, 20, PINK);
            break;

        default:
            DrawTexture(backgroundTexture, 0, 0, WHITE);
            const char* msg = "MODO TABULEIRO - Pressione SPACE para rolar o dado";
            DrawText(msg, GetScreenWidth()/2 - MeasureText(msg, 20)/2, 20, 20, BLACK);
            DrawText(TextFormat("DADO %d", _dice), 961, 58, 20, DARKGRAY);
            DrawText(TextFormat("Vez do Jogador %d", _currentPlayerIndex + 1), 540, 900, 20, BLACK);

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
    _boardState = CAN_PLAY;
}

void setPlayerSpriteAnimation(Player *player)
{
    Vector2 targetPos = player->currentTile->next->position;
    Vector2 direction = Vector2Subtract(targetPos, player->position);
    direction = Vector2Normalize(direction);
    TraceLog(LOG_INFO,"(%f %f)", direction.x, direction.y);

    float dx = direction.x;
    float dy = direction.y;
    
    // Tolerância para considerar movimento horizontal/vertical
    const float tolerance = 0.3f;
    
    if (fabs(dy) < tolerance)  // Movimento principalmente horizontal
    {
        player->sprite = playerWalkSideSheet[currentSpriteIndex];
        // Aplica flip horizontal se estiver indo para a esquerda
        if (dx < 0) 
        {
            player->flipHorizontal = true;
        }
        else
        {
            player->flipHorizontal = false;
        }
    }
    else if (dy < -tolerance)  // Movimento para cima
    {
        player->sprite = playerWalkBackSheet[currentSpriteIndex];
        player->flipHorizontal = false;
    }
    else if (dy > tolerance)   // Movimento para baixo
    {
        //player->sprite = playerWalkFrontSheet[currentSpriteIndex];
        player->flipHorizontal = false;
    }
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

    if (distance < 3.0f) {
        // Chegou ao próximo tile
        player->position = targetPos;
        player->prevTile = player->currentTile;
        player->currentTile = player->nextTile;

        // Verifica se chegou ao destino final
        if (player->currentTile == player->targetTile) {
            player->nextTile = NULL;
            return true;
        }

        // Define o próximo tile novamente
        player->nextTile = forward ? player->currentTile->next : player->currentTile->prev;
        if (player->nextTile == NULL) return true;
    } else {
        // Movimento suave
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
        p->flipHorizontal? -1 * _playerTextSrc.width * PLAYER1_TEXT_SCALE: _playerTextSrc.width * PLAYER1_TEXT_SCALE, 
        _playerTextSrc.height * PLAYER1_TEXT_SCALE
    };
    DrawTexturePro(p->sprite, _playerTextSrc, _playerTextDest, (Vector2){0, 0}, 0, WHITE);
    
    const char* playerLabel = TextFormat("P%d", p->number);
    int labelWidth = MeasureText(playerLabel, 20);
    DrawText(playerLabel,
             playerCenter.x - labelWidth/2,  
             p->position.y - 20,             
             10, 
             p->color);
}

void freeBoard() {
    // Libera texturas e sons
    UnloadTexture(backgroundTexture);
    UnloadTexture(_playerText);
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