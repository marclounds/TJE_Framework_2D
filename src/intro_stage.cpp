#include "intro_stage.h"
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gameMap.h"
#include <vector>
#include <cmath>



IntroStage::IntroStage()
{
    title_screen.load("data/title_screen.png");
}

void IntroStage::update(double dt)
{
    // Intro screen logic
    
    if (Input::wasKeyPressed(SDL_SCANCODE_A))
    {
        finished = true;
    }
}

void IntroStage::render(Image &framebuffer)
{
    // Draw intro screen
    framebuffer.drawImage(
        title_screen,
        0,
        0,
        framebuffer.width,
        framebuffer.height
    );
    
    


}