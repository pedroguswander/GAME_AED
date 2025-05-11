#include "board.h"
#include "raylib.h"
#include "raymath.h"
#include "time.h"
#include "string.h"

BoardState _boardState = CAN_PLAY;
Tile _tiles[3] = {0};
Player _player = {0};
Rectangle tileRects[3] = {0};

int _dice;
int _targetTile = 0;
int _acertou = 0;

const char* tileLabels[3] = {
    "Inicio",
    "Casa AED",
    "Casa INFRA SO"
};

void createBoard()
{
    for (int i = 0; i < 3; i ++)
    {
        _tiles[i].position = (Vector2) {200+(TILE_DISTANCE*i), 600};
        _tiles[i].type = QUESTION;
        strcpy(_tiles[i].topic, tileLabels[i]); 
    }

    tileRects[0] = (Rectangle) {_tiles[0].position.x - 50, _tiles[0].position.y - 50, 100, 100};
    tileRects[1] = (Rectangle) {_tiles[1].position.x - 50, _tiles[1].position.y - 50, 100, 100};
    tileRects[2] = (Rectangle) {_tiles[2].position.x - 50, _tiles[2].position.y - 50, 100, 100};

    _player =  (Player) {
        _tiles->position,
        (Vector2) {0,0},
        0,
        0
    };

}

void updateBoard()
{
    switch (_boardState)
    {
    case CAN_PLAY:
        if (IsKeyPressed(KEY_SPACE)) { //lidarcomJogada()
            _dice = rand() % 2 + 1;
            _targetTile = _player.currentTile + _dice;
            if (_targetTile > 2) _targetTile = 2;
            _player.prevPosition = _player.position;
            _player.prevTile = _player.currentTile;
            _boardState = MOVING;
        }
        break;
    
    case MOVING:
        Vector2 targetPos = _tiles[_targetTile].position;
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

    // Desenhar casas
    for (int i = 0; i < 3; i++) {
        DrawRectangleRec(tileRects[i], LIGHTGRAY);
        DrawRectangleLinesEx(tileRects[i], 2, DARKGRAY);
        DrawText(tileLabels[i], tileRects[i].x + 10, tileRects[i].y + 40, 16, BLACK);
    }

    // Desenhar jogador
    DrawCircleV(_player.position, 20, RED);
}