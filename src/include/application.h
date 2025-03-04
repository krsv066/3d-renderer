#pragma once

#include "renderer.h"
#include "world.h"

class Application {
public:
    Application();
    void Run();

private:
    Renderer renderer_;
    World world_;
    Camera camera_;
};
