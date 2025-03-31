#include "application.h"
#include "camera.h"
#include "cmd_parser.h"
#include "obj_parser.h"
#include "timer.h"

namespace renderer {
Application::Application(int argc, char* argv[])
    : renderer_(),
      world_(ObjParser::CreateObjects(CmdParser::ExtractModelInfo(argc, argv))),
      camera_(kDefaultWidth, kDefaultHeight),
      timer_(),
      drawer_(kDefaultWidth, kDefaultHeight, kDefaultTitel),
      screen_(std::make_unique<Screen>(kDefaultWidth, kDefaultHeight)) {
}

Movement Application::ProcessInput(double delta_time) {
    Movement movement;
    const double distance = kDistanceScale * delta_time;
    const double rotation = kRotationScale * delta_time;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        movement.forward_movement = distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
        movement.forward_movement = -distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
        movement.right_movement = distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        movement.right_movement = -distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
        movement.up_movement = distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl)) {
        movement.up_movement = -distance;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        movement.horizontal_rotation = rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        movement.horizontal_rotation = -rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        movement.vertical_rotation = rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        movement.vertical_rotation = -rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q)) {
        movement.roll_rotation = rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E)) {
        movement.roll_rotation = -rotation;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
        renderer_.SetMode(Renderer::Mode::Wireframe);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
        renderer_.SetMode(Renderer::Mode::Filled);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape)) {
        drawer_.CloseWindow();
    }

    return movement;
}

void Application::Run() {
    while (drawer_.IsWindowOpen()) {
        while (const std::optional event = drawer_.PollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                drawer_.CloseWindow();
            }
        }

        double delta_time = timer_.Tick();
        Movement movement = ProcessInput(delta_time);
        camera_.Move(movement);
        screen_ = renderer_.Render(world_, camera_, std::move(screen_));
        drawer_.Draw(screen_);
    }
}
}  // namespace renderer
