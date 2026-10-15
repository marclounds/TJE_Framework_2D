#pragma once

#include "stage.h"
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "gameState.h"

// Forward declaration to avoid requiring play_stage.h here

class LoadStage : public Stage
{
public:

    LoadStage(GameState& gamestate);
    void update(double dt) override;
    void render(Image& framebuffer) override;
    int numSaves;
    int selectedSave;
    bool finished = false;
    void loadGame(GameState& gamestate, int saveNumber);
    void getSavedGames();

    std::vector<std::string> savedGames;

private:
    Image end_screen;
    GameState& gamestate;
};