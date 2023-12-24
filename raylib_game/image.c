#include "image.h"

Texture2D PathToTexture(char* path,int size)
{
	Image image = LoadImage(path);
	ImageResizeNN(&image, size, size);
	return LoadTextureFromImage(image);
}

Texture2D HollowTile(enum TileType type)
{
    switch (type)
    {
    case Empty:
        break;
    case Ice:
        return PathToTexture("image/tiles/ice_bg.png",GRID_SIZE);
        break;
    case Grass:
        return PathToTexture("image/tiles/grass_bg.png", GRID_SIZE);
        break;
    case Clok:
        return PathToTexture("image/tiles/clok_bg.png", GRID_SIZE);
        break;
    case Stone:
        return PathToTexture("image/tiles/stone_bg.png", GRID_SIZE);
        break;
    case Sand:
        return PathToTexture("image/tiles/sand_bg.png", GRID_SIZE);
        break;
    case Igneous:
        return PathToTexture("image/tiles/igneous_bg.png", GRID_SIZE);
        break;
    case Lava:
        return PathToTexture("image/tiles/lave_bg.png", GRID_SIZE);
        break;
    default:
        break;
    }
}