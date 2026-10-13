#pragma once
#include "stage.h"
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"

class IntroStage : public Stage
{
public:

    IntroStage();
    void update(double dt) override;
    void render(Image& framebuffer) override;

    bool finished = false;

private:
    Image title_screen;
};