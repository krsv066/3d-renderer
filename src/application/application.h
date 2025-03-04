#pragma once

#include "renderer.h"

namespace renderer {
class Application {
public:
    Application();
    void Run();

private:
    Renderer renderer_;
    World world_;
    Camera camera_;
};
}  // namespace renderer
