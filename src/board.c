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


#define BOARD_SIZE 15
#define PLAYER1_TEXT_SIZE 32
#define PLAYER1_TEXT_SCALE 3
#define MAX_PLAYERS 2

Texture2D backgroundTexture;
BoardState _boardState = CAN_PLAY;

Sound victoyTheme;

Tile *_tilesHEAD = NULL;
Tile *_tilesTAIL = NULL;

Player _players[MAX_PLAYERS];

Texture2D _playerText = {0};
Rectangle _player1TextSrc = {0};
Rectangle _player1TextDest = {0};

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
    "Início",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Matemática",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Conhecimentos Gerais",
    "Filmes",
    "Conhecimentos Gerais",
    "Matemática",
    "Conhecimentos Gerais",
    "Músicas",
    "Filmes",
};


void *loadQuestionThread(void *arg) {
    Tile *tile = (Tile *)arg;
    const char (*themes)[100] = getThemesOfTopic(tile->topic);

    if (themes != NULL) {
        _questionTile = addQuestion(tile->topic, themes[0]);
    } else {
        _questionTile = (Question){0};
    }

    _loadingFinishedBoard = true;
    return NULL;
}


void createTile(TileType type, const char *topic, int tile)
{
    Tile *new_tile = (Tile *) malloc(sizeof(Tile));
    if (new_tile) 
    {
        new_tile->position = tilePositions[tile];
        new_tile->type = type;
        new_tile->rect = (Rectangle) {
            new_tile->position.x - 50,
            new_tile->position.y - 50,
            100,
            100
        };
        strcpy(new_tile->topic, topic);
        new_tile->tile = tile;
        new_tile->next = NULL;
        new_tile->prev = NULL;

        if (_tilesHEAD == NULL)
        {
            _tilesHEAD = new_tile;
            _tilesTAIL = new_tile;
        }
        else 
        {
            _tilesTAIL->next = new_tile;
            new_tile->prev = _tilesTAIL;
            _tilesTAIL = new_tile;
        }
    }
}


void createBoard()
{
    InitAudioDevice();

    _boardState = WAITING_TO_END;
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;
    backgroundTexture = LoadTexture("background-board-mode-1.png");

    victoyTheme = LoadSound("music/videoplayback.wav");

    _playerText = LoadTexture("player/hero-idle-front.png");
    _player1TextSrc = (Rectangle) {0, 0, PLAYER1_TEXT_SIZE, PLAYER1_TEXT_SIZE};

    for (int i = 0; i < 4; i++) {
        optionRects[i] = (Rectangle){ 50, startY + i * 60, 800, 40};
        options[i].rect = optionRects[i];
        strcpy(options[i].answer, labels[i]);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        createTile(QUESTION, tileLabels[i], i);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (i == 0)
        {
        _players[i] = (Player){
            _tilesHEAD->position,
            (Vector2){0, 0},
            _tilesTAIL->prev->prev,
            NULL,
            NULL,
        };
        }
        else 
        {
            _players[i] = (Player){
            _tilesHEAD->position,
            (Vector2){0, 0},
            _tilesHEAD,
            NULL,
            NULL,
        };
        }

    }

}

Vector2 getPositionOfTile(int tile)
{
    Tile *iterador = _tilesHEAD;
    while (iterador != NULL)
    {
        if (iterador->tile == tile)
        {
            return iterador->position;
        }
        iterador = iterador->next;
    }
    return (Vector2){0};
}

Tile *getTileByTile(int tile) {
    Tile *iterador = _tilesHEAD;
    while (iterador != NULL) {
        if (iterador->tile == tile) {
            return iterador;
        }
        iterador = iterador->next;
    }
    return NULL; 
}

int getIndexOfTile(Tile *tile) {
    if (tile == NULL) return -1;

    Tile *current = _tilesHEAD;
    int index = 0;
    
    while (current != NULL) {
        if (current == tile) { 
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; 
}


void updateBoard()
{
    Player *player = &_players[_currentPlayerIndex];

    _playerColor = _currentPlayerIndex? BLUE: RED;

    switch (_boardState)
    {
        case CAN_PLAY:
            if (IsKeyPressed(KEY_SPACE)) {
                _dice = rand() % 3 + 1;
                int _currentTileIndex = getIndexOfTile(player->currentTile);
                int destino = _currentTileIndex + _dice;

                if (destino >= BOARD_SIZE) destino = BOARD_SIZE - 1;
                player->targetTile = getTileByTile(destino);

                tileBeforePlaying = player->currentTile;
                _boardState = MOVING;
            }
            break;

        case MOVING:
            if (!movePlayer(player, true)) break;

            if (player->currentTile && player->currentTile->type == QUESTION) {
                _loadingFinishedBoard = false;
                pthread_create(&_loadThread, NULL, loadQuestionThread, player->currentTile);
                _boardState = LOADING;
            } else {
                finalizeTurn();
            }
            break;

        case MOVING_BACKWARDS:
            if (!movePlayer(player, false)) break;

            finalizeTurn();
            break;


        case LOADING:
            if (_loadingFinishedBoard) {
                pthread_join(_loadThread, NULL);
                _boardState = EVENT;
            }
            break;

        case END:
            PlaySound(victoyTheme);

            //mexer em _menuOption
            break;

        case WAITING_TO_END:
            waitingToEndTimer += GetFrameTime();
            if (waitingToEndTimer >= waitingToEndDuration)
            {
                waitingToEndTimer = 0.0f;
                _boardState = END;
            }
            break;


        case EVENT:
            TraceLog(LOG_INFO, "Tile atual do jogador: %d", player->currentTile->tile);
            
            if (_tilesHEAD != NULL &&_tilesHEAD->type == QUESTION)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    Vector2 mouse = GetMousePosition();
                    for (int i = 0; i < 4; i++) {
                        if (CheckCollisionPointRec(mouse, options[i].rect)) {
                            if (strcmp(options[i].answer, _questionTile.answer) == 0)
                            {
                                _gotItRigth = true;
                            }
                            else 
                            {
                                _gotItRigth = false;
                            }

                            _boardState = SHOW_ANSWER;
                        }
                    }
                }
            }


            break;

        case SHOW_ANSWER:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), nextQuestionButton))
            {
                if (_gotItRigth)
                {
                    finalizeTurn();
                    if (player->currentTile->tile >= BOARD_SIZE -1)
                    {
                        _boardState = WAITING_TO_END;
                    }
                }
                else
                {
                    player->targetTile = tileBeforePlaying;
                    _boardState = MOVING_BACKWARDS;
                }
            }

        default:
            break;
    }
}

void freeBoard() {
    UnloadSound(victoyTheme); 

    Tile *atual = _tilesHEAD;
    while (atual != NULL) {
        Tile *prox = atual->next;
        free(atual);
        atual = prox;
    }
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;
}

void drawBoard()
{
    switch (_boardState)
    {
        case LOADING:
            DrawText("Carregando pergunta...", GetScreenWidth()/2 - 100, GetScreenHeight()/2, 30, BLUE);
            break;


        case EVENT:
            drawQuestion(options, _questionTile);
            DrawText(TextFormat("P%d", _currentPlayerIndex + 1), 540, 900, 32, _currentPlayerIndex? BLUE: RED);
            break;

        case END:
            DrawRectangleRec((Rectangle) {0, 0, 1920, 1080}, BLUE);
            DrawText(TextFormat("Jogador: %d venceu!!", _currentPlayerIndex+1),
            1920/2, 1080/2, 20, PINK);
            break;

        case SHOW_ANSWER:
            DrawText("Gabarito:", 50, 100, 28, YELLOW);
            DrawText(_questionTile.answer, 200, 100, 28, WHITE);
            DrawText(_gotItRigth ? "ACERTOU!" : "ERROU!", 50, 200, 40, _gotItRigth ? GREEN : RED);
            DrawRectangleRec(nextQuestionButton, RED);
            DrawText("CONTINUAR", nextQuestionButton.x + 5, nextQuestionButton.y + 5, 16, WHITE);
            break;
        
        default:
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Elementos principais da interface do modo tabuleiro
            const char* mensagem = "MODO TABULEIRO - Pressione SPACE para rolar o dado";
            DrawText(mensagem, GetScreenWidth()/2 - MeasureText(mensagem, 20)/2, 20, 20, BLACK);
            DrawText(TextFormat("DADO %d", _dice), 961, 58, 20, DARKGRAY);
            DrawText(TextFormat("%d", _acertou), 20, 60, 20, DARKGRAY);
            DrawText(TextFormat("Vez do Jogador %d", _currentPlayerIndex + 1), 540, 900, 20, BLACK);

            for (int i = 0; i < MAX_PLAYERS; i++) {
                Player *p = &_players[i];
                _player1TextDest = (Rectangle){
                    p->currentTile->position.x + i * 40,  // Deslocamento para evitar sobreposição
                    p->currentTile->position.y,
                    _player1TextSrc.width * PLAYER1_TEXT_SCALE,
                    _player1TextSrc.height * PLAYER1_TEXT_SCALE
                };
                DrawTexturePro(_playerText, _player1TextSrc, _player1TextDest, (Vector2){0, 0}, 0, WHITE);
                DrawText(TextFormat("P%d", _currentPlayerIndex + 1), _player1TextDest.x, _player1TextDest.y+100,
                 20, i? BLUE: RED);
            }

            if (_tilesHEAD != NULL) {
                Tile *current = _tilesHEAD;
                int i = 0;
                while (current != NULL) {
                    DrawRectangleRec(current->rect, LIGHTGRAY);
                    DrawRectangleLinesEx(current->rect, 2, DARKGRAY);
                    DrawText(current->topic, current->rect.x + 10, current->rect.y + 40, 16, BLACK);
                    current = current->next;
                    i++;
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

bool movePlayer(Player *player, bool forward) {
    if (!isWaiting) {
        isWaiting = true;
        stepTimer = 0.0f;
        return false;
    }

    stepTimer += GetFrameTime();
    if (stepTimer >= stepDelay) {
        player->prevTile = player->currentTile;
        player->currentTile = forward ? player->currentTile->next : player->currentTile->prev;

        Vector2 direction = Vector2Subtract(player->prevTile->position, player->currentTile->position);
        direction = Vector2Normalize(direction);
        Vector2 movement = Vector2Scale(direction, 5.0f);

        player->currentTile->position = Vector2Add(player->currentTile->position, movement);

        isWaiting = false;
    }

    return player->currentTile == player->targetTile;
}



