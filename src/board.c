#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "time.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define BOARD_SIZE 20

Texture2D backgroundTexture;
BoardState _boardState = CAN_PLAY;
//Tile _tiles[3] = {0};
Tile *_tilesHEAD = NULL;
Tile *_tilesTAIL = NULL;
Player _player = {0};
//Rectangle tileRects[3] = {0};

int _dice;
int _targetTile = 0;
int _acertou = 0;

const char* tileLabels[BOARD_SIZE] = {
    "Início",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Matemática",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Matemática",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Matemática",
    "Conhecimentos Gerais",
    "Filmes",
    "Músicas",
    "Matemática",
    "Boss Node AED",
    "Boss Node AED",
    "Final"
};


void createTile(TileType type, const char *topic, int tile)
{
    Tile *new_tile = (Tile *) malloc(sizeof(Tile));
    if (new_tile) 
    {
        new_tile->position = (Vector2) {200 + (TILE_DISTANCE * tile), 600};
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
    _boardState = CAN_PLAY;
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;
    backgroundTexture = LoadTexture("board-background.png");

    for (int i = 0; i < BOARD_SIZE; i++) {
        createTile(QUESTION, tileLabels[i], i);
    }

    _player = (Player) {
        _tilesHEAD->position,
        (Vector2) {0,0},
        0,
        0
    };
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

void updateBoard()
{
    switch (_boardState)
    {
        case CAN_PLAY:
            if (IsKeyPressed(KEY_SPACE)) {
                _dice = rand() % 2 + 1;
                _targetTile = _player.currentTile + _dice;
                if (_targetTile >= BOARD_SIZE) _targetTile = BOARD_SIZE - 1;
                _player.prevPosition = _player.position;
                _player.prevTile = _player.currentTile;
                _boardState = MOVING;
            }
            break;

        case MOVING: {
            Vector2 targetPos = getPositionOfTile(_targetTile);
            Vector2 direction = Vector2Subtract(targetPos, _player.position);
            float distance = Vector2Length(direction);

            if (distance < 2.0f) {
                _player.position = targetPos;
                _player.currentTile = _targetTile;
                _acertou = rand() % 2;

                if (!_acertou) {
                    targetPos = _player.prevPosition;
                    _targetTile = _player.prevTile;
                    _boardState = MOVING;
                } else {
                    _boardState = CAN_PLAY;
                }

            } else {
                direction = Vector2Normalize(direction);
                _player.position.x += direction.x * 5;
                _player.position.y += direction.y * 5;
            }

            break;
        }

        default:
            break;
    }
}
void freeBoard() {
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
    // Desenha a imagem de fundo
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    // Elementos principais da interface do modo tabuleiro
    const char* mensagem = "MODO TABULEIRO - Pressione SPACE para rolar o dado";
    DrawText(mensagem, GetScreenWidth()/2 - MeasureText(mensagem, 20)/2, 20, 20, BLACK);
    DrawText(TextFormat("DADO %d", _dice), 20, 40, 20, DARKGRAY);
    DrawText(TextFormat("%d", _acertou), 20, 60, 20, DARKGRAY);

    if (_tilesHEAD != NULL) {
        Tile *current = _tilesHEAD;
        int i = 0;
        while (current != NULL) {
            DrawRectangleRec(current->rect, LIGHTGRAY);
            DrawRectangleLinesEx(current->rect, 2, DARKGRAY);
            DrawText(current->topic, current->rect.x + 10, current->rect.y + 40, 16, BLACK);

            if (_player.currentTile == i) {
                DrawCircleV(_player.position, 20, RED);
            }

            current = current->next;
            i++;
        }
    }
}
