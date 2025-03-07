#pragma once

#include "renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace renderer {
class Application {
public:
    Application();
    void Run();

private:
    Renderer renderer_;
    World world_;
    Camera camera_;

    void ProcessInput(sf::RenderWindow& window, float delta_time);
};
}  // namespace renderer
