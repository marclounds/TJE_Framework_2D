#include "gamemap.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
GameMap::GameMap(int w, int h)
{
    width = w;
    height = h;

    tile_width = 0;
    tile_height = 0;
    layers = nullptr;
}

sCell& GameMap::getCell(int x, int y, int layer)
{
    int index = x + y * width;

    return layers[layer].data[index];
}


//It reads a json file and creates a GameMap object from it

GameMap* loadGameMap(const char* filename)
{
    using json = nlohmann::json;

    std::ifstream f(filename);
    //It checks whether opening the file was successful. If not, it returns a null pointer.



    if (!f.good()) {
        std::cout << "ERROR: Could not open map: " << filename << std::endl;
        return nullptr;

    }

    json jData = json::parse(f);
    //It reads the width and height of the map from the JSON data, as well as the number of layers.
    int w = jData["width"];
    int h = jData["height"];
    int numLayers = jData["layers"].size();

    GameMap* map = new GameMap(w, h);
    map->num_layers = numLayers;

    map->layers = new sLayer[numLayers];

    map->tile_width = jData["tilewidth"];
    map->tile_height = jData["tileheight"];

    for (int l = 0; l < numLayers; l++)
    {

        //We dinamically allocate space for the cells of each of the layers
        map->layers[l].data = new sCell[w * h];
        //For each of the layers it reads the data from the JSON and fills the corresponding cells in the GameMap object.
        json layer = jData["layers"][l];

        for (int x = 0; x < map->width; x++)
        {
            for (int y = 0; y < map->height; y++)
            {
                int index = x + y * map->width;

                int type =
                    layer["data"][index].get<int>() - 1;

                map->getCell(x, y, l).type = type;
            }
        }
    }

    return map;
}