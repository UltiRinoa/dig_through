#ifndef DIG_H
#define DIG_H
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#define G 500
#define PLAYER_JUMP_SPD 150.0f
#define PLAYER_HOR_SPD 200.0f
#define PLAYER_SIZE 24

#define GRID_SIZE 40
#define GRID_COUNT 36

typedef enum TileType
{
    Empty,
    Ice,
    Grass,
    Clok,
    Stone,
    Sand,
    Igneous,
    Lava
}TileType;

typedef enum GravityArea
{
    North,
    East,
    West,
    South
}GravityArea;

typedef struct GridPosition
{
    int x;
    int y;
}GridPosition;

typedef struct Player {
    Vector2 position;
    Vector2 size;
    Texture2D texture;
    GravityArea gravityArea;
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
    TileType type;
    bool isHollowed;
}Tile;

Rectangle ToRectangle(Vector2 position, int size);

#endif