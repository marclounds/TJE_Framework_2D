#include "end_stage.h"
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gameMap.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>



PauseStage::PauseStage(GameState& state)
    :gamestate(state)
{
    end_screen.load("data/pause.png");
    NumSaves = 0;
}

void PauseStage::update(double dt)
{
    // Intro screen logic
    if (Input::wasKeyPressed(SDL_SCANCODE_A))
    {
        finished = true;
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
        saveGame(gamestate);

    }
    if (Input::wasKeyPressed(SDL_SCANCODE_L)) {
        load = true;
    }
}

void PauseStage::render(Image& framebuffer)
{
    // Draw intro screen
    framebuffer.drawImage(
        end_screen,
        0,
        0,
        framebuffer.width,
        framebuffer.height
    );
        
}

void PauseStage::saveGame(GameState& gamestate)
{
    std::filesystem::create_directories("saves");

    std::string filename;

    // Find an available save number
    do
    {
        filename = "saves/save" + std::to_string(NumSaves) + ".txt";

        if (std::filesystem::exists(filename))
            NumSaves++;
        else
            break;

    } while (true);

    std::ofstream file(filename);
    //Before saving we determine how many enemies we currently have so that when we load we allocate right amount of enemy storage
    

    if (file.is_open())
    {
        file << gamestate.CharacterPos.x << "\n";
        file << gamestate.CharacterPos.y << "\n";
        file << gamestate.score << "\n";
        file << gamestate.keyHas << "\n";
        file << gamestate.doorOpen << "\n";
        file << gamestate.enemies.size() << "\n";


        //For enemies in the state
        for (Enemy& enemy : gamestate.enemies) {
            file << enemy.x << "\n";
            file << enemy.y << "\n";
            file << enemy.dirX << "\n";
            file << enemy.dirY << "\n";
            file << enemy.dead << "\n";
            file << enemy.directionTimer << "\n";
            file << enemy.initialPosX << "\n";
            file << enemy.initialPosY << "\n";

        }

        std::cout << "Game saved as " << filename << std::endl;

        NumSaves++;
    }
    else
    {
        std::cout << "Error saving game!" << std::endl;
    }
}
//
//
////to load game 
//void PauseStage::loadGame(PlayStage& gamestate, int saveNumber)
//{
//    std::string filename = "saves/save" + std::to_string(saveNumber) + ".txt";
//
//    std::ifstream file(filename);
//
//    if (file.is_open())
//    {
//        float x, y;
//        int loadedScore;
//        bool loadedKey, loadedDoor;
//
//        if (file >> x >> y >> loadedScore >> loadedKey >> loadedDoor)
//        {
//            gamestate.CharacterPos.x = x;
//            gamestate.CharacterPos.y = y;
//            gamestate.score = loadedScore;
//            gamestate.keyHas = loadedKey;
//            gamestate.doorOpen = loadedDoor;
//
//            std::cout << "Game loaded from " << filename << std::endl;
//        }
//        else
//        {
//            std::cout << "Error reading save file!" << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << "Save file not found!" << std::endl;
//    }
//}