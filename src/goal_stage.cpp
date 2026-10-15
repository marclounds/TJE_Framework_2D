#include "end_stage.h"
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gameMap.h"
#include <vector>
#include <cmath>



GoalStage::GoalStage()
{
    end_screen.load("data/goal_screen.png");
}

void GoalStage::update(double dt)
{
    // Intro screen logic
    if (Input::wasKeyPressed(SDL_SCANCODE_A))
    {
        finished = true;
    }
}

void GoalStage::render(Image& framebuffer)
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