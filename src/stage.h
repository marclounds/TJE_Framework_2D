#pragma once
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"

class Stage
{
public:
    virtual void update(double dt) = 0;
    virtual void render(Image& framebuffer) = 0;

    virtual ~Stage() = default;
};