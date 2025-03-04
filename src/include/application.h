#pragma once

#include "renderer.h"
#include "screen.h"
#include "world.h"

class Application {
public:
    Application();

    void Run();

private:    
    World scene_;
    Camera camera_;
    Screen screen_;
    Renderer renderer_;
};
