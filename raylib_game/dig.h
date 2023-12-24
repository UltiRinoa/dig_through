#pragma once
#include "raylib.h"
#include "raymath.h"
#include "image.h"

#define G 500
#define PLAYER_JUMP_SPD 200.0f
#define PLAYER_HOR_SPD 200.0f
#define PLAYER_SIZE 32

#define GRID_SIZE 40
#define GRID_COUNT 36

enum TileType
{
    Empty,
    Ice,
    Grass,
    Clok,
    Stone,
    Sand,
    Igneous,
    Lava
};

typedef struct GridPosition
{
    int x;
    int y;
}GridPosition;

typedef struct Player {
    Vector2 position;
    Vector2 size;
    Texture2D texture;
    GridPosition grid;
    float speed;
    bool canJump;
    bool canDig;
} Player;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

typedef struct Tile
{
    Vector2 position;
    Texture2D texture;
    GridPosition grid;
    enum TileType type;
    bool isHollowed;
}Tile;

Rectangle ToRectangle(Vector2 position, int size);
