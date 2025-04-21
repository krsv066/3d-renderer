#pragma once

#include "screen.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace renderer {
class Drawer {
public:
    Drawer(Width width, Height height, const std::string& title);
    void Draw(const std::unique_ptr<Screen>& screen);
    bool IsWindowOpen() const;
    void CloseWindow();
    std::optional<sf::Event> PollEvent();

private:
    sf::RenderWindow window_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};
}  // namespace renderer
