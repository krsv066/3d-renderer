#pragma once

#include "renderer.h"
#include "timer.h"
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
    Timer timer_;

    void ProcessInput(sf::RenderWindow& window, double delta_time);
};
}  // namespace renderer
