#pragma once

struct sCell
{
    int type;
};

struct sLayer
{
    sCell* data;
};

class GameMap
{
public:
    int width;
    int height;

    int tile_width;
    int tile_height;
    int num_layers;
    sLayer* layers;

    GameMap(int w, int h);

    sCell& getCell(int x, int y, int layer);
};

// Separate function
GameMap* loadGameMap(const char* filename);
#pragma once
