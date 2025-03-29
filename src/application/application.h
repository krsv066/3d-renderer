#pragma once

#include "renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace renderer {
class Application {
public:
    Application(int argc, char* argv[]);
    void Run();

private:
    Renderer renderer_;
    World world_;
    Camera camera_;

    void ProcessInput(sf::RenderWindow& window, double delta_time);
};
}  // namespace renderer
