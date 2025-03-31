#include "drawer.h"

namespace renderer {
Drawer::Drawer(Width width, Height height, const std::string& title)
    : window_(sf::VideoMode({static_cast<uint32_t>(width), static_cast<uint32_t>(height)}), title),
      texture_(sf::Vector2u(width, height)),
      sprite_(texture_) {
}

void Drawer::Draw(const std::unique_ptr<Screen>& screen) {
    texture_.update(screen->GetFrameBuffer());
    sprite_.setTexture(texture_);

    window_.clear();
    window_.draw(sprite_);
    window_.display();
}

bool Drawer::IsWindowOpen() const {
    return window_.isOpen();
}

void Drawer::CloseWindow() {
    window_.close();
}

std::optional<sf::Event> Drawer::PollEvent() {
    return window_.pollEvent();
}
}  // namespace renderer
