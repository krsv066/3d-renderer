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
static constexpr double kDistanceScale = 2.5;
static constexpr double kRotationScale = 1.0;

Application::Application()
    : renderer_(Renderer::Mode::Filled),
      world_{parser::Parser::LoadObj("../build/cow.obj")},
      camera_(kDefaultWidth, kDefaultHeight) {

    world_.AddLight(Light::Directional(linalg::Vector3(-1.0, -1.0, -1.0),
                                       Color{Red{1.0}, Green{0.9}, Blue{0.7}}, 0.6));
}

void Application::ProcessInput(sf::RenderWindow& window, double delta_time) {
    const double distance = kDistanceScale * delta_time;
    const double rotation = kRotationScale * delta_time;

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
        camera_.RotateHorizontal(-rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        camera_.RotateHorizontal(rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        camera_.RotateVertical(rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        camera_.RotateVertical(-rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q)) {
        camera_.RotateRoll(rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E)) {
        camera_.RotateRoll(-rotation);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
        renderer_.SetMode(Renderer::Mode::Wireframe);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
        renderer_.SetMode(Renderer::Mode::Filled);
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
