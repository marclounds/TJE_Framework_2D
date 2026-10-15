#pragma once

#include "stage.h"
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "gameState.h"

//// Forward declaration to avoid including play_stage.h here
//class PlayStage;

class PauseStage : public Stage
{
public:

    PauseStage(GameState& state);
    void update(double dt) override;
    void render(Image& framebuffer) override;
    int NumSaves;
    bool finished = false;
    bool load = false;

    void saveGame(GameState& gamestate);
    //void loadGame(GameState& gamestate, int saveNumber);

private:
    Image end_screen;
    GameState& gamestate;
};
