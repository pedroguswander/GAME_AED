#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "time.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

BoardState _boardState = CAN_PLAY;
//Tile _tiles[3] = {0};
Tile *_tilesHEAD = NULL;
Tile *_tilesTAIL = NULL;
Player _player = {0};
//Rectangle tileRects[3] = {0};

int _dice;
int _targetTile = 0;
int _acertou = 0;

const char* tileLabels[3] = {
    "Inicio",
    "Casa AED",
    "Casa INFRA SO"
};

void criarTile(TileType type, const char *topic, int casa)
{
    Tile *novo = (Tile *) malloc(sizeof(Tile));
    if (novo) 
    {
        novo->position = (Vector2) {200+(TILE_DISTANCE*casa), 600};
        novo->type = type;
        novo->rect = (Rectangle) {
            novo->position.x - 50,
            novo->position.y - 50,
            100,
            100
        };
        strcpy(novo->topic, topic);
        novo->casa = casa;

        if (_tilesHEAD == NULL)
        {
            _tilesHEAD = novo;
            _tilesTAIL = novo;
            novo->next = _tilesHEAD;
            novo->prev = _tilesTAIL;
        }
        else {
            novo->next = _tilesHEAD;
            novo->prev = _tilesTAIL;
            novo->prev->next = novo;
            novo->next->prev = novo;
            _tilesTAIL = novo;
        }

    }
}

void createBoard()
{
    _boardState = CAN_PLAY;
    _tilesHEAD = NULL;
    _tilesTAIL = NULL;

    criarTile(QUESTION, tileLabels[0], 0);
    criarTile(QUESTION, tileLabels[1], 1);
    criarTile(QUESTION, tileLabels[2], 2);

    _player =  (Player) {
        _tilesHEAD->position,
        (Vector2) {0,0},
        0,
        0
    };

}

Vector2 getPositionOfTile(int casa)
{
    if (_tilesHEAD != NULL)
    {
        Tile *iterador = _tilesHEAD;
        do {
            if (iterador->casa == casa)
            {
                return iterador->position;
            }
            iterador = iterador->next;
        }
        while (iterador != _tilesHEAD);
    }
    return (Vector2) {0};
}

void updateBoard()
{
    switch (_boardState)
    {
    case CAN_PLAY:
        if (IsKeyPressed(KEY_SPACE)) {
            _dice = rand() % 2 + 1;
            _targetTile = _player.currentTile + _dice;
            if (_targetTile > 2) _targetTile = 2;
            _player.prevPosition = _player.position;
            _player.prevTile = _player.currentTile;
            _boardState = MOVING;
        }
        break;
    
    case MOVING:
        Vector2 targetPos = getPositionOfTile(_targetTile);
        Vector2 direction = Vector2Subtract(targetPos, _player.position);
        float distance = Vector2Length(direction);
        if (distance < 2.0f) {
            _player.position = targetPos;
            _player.currentTile = _targetTile;
            _acertou = rand() % 2;

            if (!_acertou)
            {
                targetPos = _player.prevPosition;
                _targetTile = _player.prevTile;
                _boardState = MOVING;
            }
            else
            {
                _boardState = CAN_PLAY;
            }

        } else {
            direction = Vector2Normalize(direction);
            _player.position.x += direction.x * 5;
            _player.position.y += direction.y * 5;
        }

        break;


    default:
        break;
    }
}

void drawBoard()
{
    DrawText("MODO TABULEIRO - Pressione SPACE para rolar o dado", 20, 20, 20, DARKGRAY);
    DrawText(TextFormat("DADO %d", _dice), 20, 40, 20, DARKGRAY);
    DrawText(TextFormat("%d", _acertou), 20, 60, 20, DARKGRAY);

    if (_tilesHEAD != NULL) {
        Tile *current = _tilesHEAD;
        do {
            DrawRectangleRec(current->rect, LIGHTGRAY);
            DrawRectangleLinesEx(current->rect, 2, DARKGRAY);
            DrawText(current->topic, current->rect.x + 10, current->rect.y + 40, 16, BLACK);
            current = current->next;
        } while (current != _tilesHEAD);
    }

    DrawCircleV(_player.position, 20, RED);
}
