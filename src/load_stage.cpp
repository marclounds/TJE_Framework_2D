#include "end_stage.h"
#include "load_stage.h"
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gameMap.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cmath>

LoadStage::LoadStage(GameState& state)
    : gamestate(state)
{
 //We must load all the saves in a vector of Game States
    savedGames.clear();

    // Check if the saves directory exists
    if (!std::filesystem::exists("saves"))
        return;

    // Iterate through every file in the directory
    for (const auto& entry : std::filesystem::directory_iterator("saves"))
    {
        if (entry.is_regular_file() &&
            entry.path().extension() == ".txt")
        {
            savedGames.push_back(entry.path().filename().string());
        }
    }

    // Sort filenames
    std::sort(savedGames.begin(), savedGames.end());

    selectedSave = 0;

}

void LoadStage::update(double dt)
{
    // Intro screen logic
    if (Input::wasKeyPressed(SDL_SCANCODE_A))
    {

        
        loadGame(gamestate, selectedSave);
        finished = true;

    }
    if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
        selectedSave--;
        if (selectedSave < 0) {
            selectedSave = (int)savedGames.size() - 1;
        }
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
        selectedSave++;
        if (selectedSave > (int)savedGames.size() - 1) {
            selectedSave = 0; 
            
        }
    }
}

void LoadStage::render(Image& framebuffer)
{
    framebuffer.drawText("LOAD GAME", 10, 10, font);

    if (savedGames.empty())
    {
        framebuffer.drawText("No saved games", 10, 40, font);
        return;
    }

    for (int i = 0; i < savedGames.size(); i++)
    {
        std::string text = savedGames[i];

        // Highlight selected save
        if (i == selectedSave)
        {
            text = "> " + text;
        }

        framebuffer.drawText(text, 10, 40 + i * 20, font);
    }

}



//to load game 
void LoadStage::loadGame(GameState& gamestate, int saveNumber)
{
    std::string filename = "saves/save" + std::to_string(saveNumber) + ".txt";

    std::ifstream file(filename);

    if (file.is_open())
    {
        float x, y;
        int loadedScore;
        bool loadedKey, loadedDoor;

        if (file >> x >> y >> loadedScore >> loadedKey >> loadedDoor)
        {
            gamestate.CharacterPos.x = x;
            gamestate.CharacterPos.y = y;
            gamestate.score = loadedScore;
            gamestate.keyHas = loadedKey;
            gamestate.doorOpen = loadedDoor;

            std::cout << "Game loaded from " << filename << std::endl;
        }
        else
        {
            std::cout << "Error reading save file!" << std::endl;
        }
    }
    else
    {
        std::cout << "Save file not found!" << std::endl;
    }
}