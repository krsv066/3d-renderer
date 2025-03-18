#include "application.h"
#include "camera.h"
#include "obj_parser.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>

namespace renderer {
static constexpr Width kDefaultWidth{1280};
static constexpr Height kDefaultHeight{720};
static constexpr std::string kDefaultTitel = "3D Renderer";

Application::Application()
    : renderer_(Renderer::Mode::Filled),
      world_{parser::Parser::LoadObj("../build/cow.obj", 0xFFFFFF)},
      camera_(Fov{60.0 * M_PI / 180.0}, Aspect{static_cast<double>(kDefaultWidth) / kDefaultHeight},
              Near{0.1}, Far{100.0}) {
}

void Application::ProcessInput(sf::RenderWindow& window, double delta_time) {
    double distance = 2.5 * delta_time;
    double rotation_speed = 1.5 * delta_time;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        camera_.MoveForward(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
        camera_.MoveBackward(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        camera_.MoveLeft(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
        camera_.MoveRight(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
        camera_.MoveUp(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl)) {
        camera_.MoveDown(distance);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        camera_.RotateHorizontal(-rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        camera_.RotateHorizontal(rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        camera_.RotateVertical(rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        camera_.RotateVertical(-rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q)) {
        camera_.RotateRoll(rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E)) {
        camera_.RotateRoll(-rotation_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape)) {
        window.close();
    }
}

void Application::Run() {
    sf::RenderWindow window(sf::VideoMode({kDefaultWidth, kDefaultHeight}), kDefaultTitel);
    sf::Texture texture(sf::Vector2u(kDefaultWidth, kDefaultHeight));
    sf::Sprite sprite(texture);

    Screen screen(kDefaultWidth, kDefaultHeight);
    auto last_frame_time = std::chrono::high_resolution_clock::now();

    while (window.isOpen()) {
        auto current_frame_time = std::chrono::high_resolution_clock::now();
        double delta_time =
            std::chrono::duration<double>(current_frame_time - last_frame_time).count();
        last_frame_time = current_frame_time;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ProcessInput(window, delta_time);
        screen = renderer_.Render(world_, camera_, std::move(screen));
        texture.update(screen.GetFrameBuffer());
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }
}
}  // namespace renderer
