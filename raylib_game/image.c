#include "image.h"
#include <string.h>

void InitTexture(void)
{
    for (int i = 0; i < 10; i++)
    {
        char path[256];
        sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Ice), i);
        textureLib.ice.tile[i] = PathToTexture(path, GRID_SIZE);
        sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Grass), i);
        textureLib.grass.tile[i] = PathToTexture(path, GRID_SIZE);
        // sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Clok), i);
        // textureLib.clok.tile[i] = PathToTexture(path, GRID_SIZE);
        // sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Stone), i);
        // textureLib.stone.tile[i] = PathToTexture(path, GRID_SIZE);
        sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Sand), i);
        textureLib.sand.tile[i] = PathToTexture(path, GRID_SIZE);
        sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Igneous), i);
        textureLib.igneous.tile[i] = PathToTexture(path, GRID_SIZE);
        sprintf(path, "image/tiles/%s_%d.png", TileTypeToString(Lava), i);
        textureLib.lava.tile[i] = PathToTexture(path, GRID_SIZE);
    }
}

const char *TileTypeToString(TileType type)
{
    switch (type)
    {
    case Empty:
        return "empty";
    case Ice:
        return "ice";
    case Grass:
        return "grass";
    case Clok:
        return "clock";
    case Stone:
        return "stone";
    case Sand:
        return "sand";
    case Igneous:
        return "igneous";
    case Lava:
        return "lava";
    default:
        return "";
    }
}

Texture2D PathToTexture(char *path, int size)
{
    Image image = LoadImage(path);
    ImageResizeNN(&image, size, size);
    return LoadTextureFromImage(image);
}

Texture2D GetTileTexture(TileType type, int idx)
{
    switch (type)
    {
    case Empty:
        break;
    case Ice:
        return textureLib.ice.tile[idx];
    case Grass:
        return textureLib.grass.tile[idx];
    case Clok:
        return textureLib.clok.tile[idx];
    case Stone:
        return textureLib.stone.tile[idx];
    case Sand:
        return textureLib.sand.tile[idx];
    case Igneous:
        return textureLib.igneous.tile[idx];
    case Lava:
        return textureLib.lava.tile[idx];
    }
    return textureLib.ice.tile[0];
}
