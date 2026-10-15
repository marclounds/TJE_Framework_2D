#pragma once

#include "stage.h"
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"

class TutorialStage : public Stage
{
public:

    TutorialStage();
    void update(double dt) override;
    void render(Image& framebuffer) override;

    bool finished = false;

private:
    Image end_screen;
};
