#include "application.h"
#include "camera.h"
#include "obj_parser.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace renderer {
static constexpr Width kDefaultWidth{1280};
static constexpr Height kDefaultHeight{720};
static constexpr std::string kDefaultTitel = "3D Renderer";

Application::Application()
    : renderer_(Renderer::Mode::Wireframe),
      world_{parser::Parser::LoadObj("../src/models/cube.obj", Vector3(0.0, 0.0, -3.0),
                                     AngleAxis(M_PI / 8.0, Vector3::UnitX()).toRotationMatrix() *
                                         AngleAxis(M_PI / 8.0, Vector3::UnitY()).toRotationMatrix(),
                                     0xFF00FF)},
      camera_(Fov{60.0 * M_PI / 180.0}, Aspect{static_cast<double>(kDefaultWidth) / kDefaultHeight},
              Near{0.1}, Far{100.0}) {
}

void Application::Run() {
    sf::RenderWindow window(sf::VideoMode({kDefaultWidth, kDefaultHeight}), kDefaultTitel);
    sf::Texture texture(sf::Vector2u(kDefaultWidth, kDefaultHeight));
    sf::Sprite sprite(texture);

    Screen screen(kDefaultWidth, kDefaultHeight);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        screen = renderer_.Render(world_, camera_, std::move(screen));

        texture.update(screen.GetFrameBuffer());
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }
}
}  // namespace renderer
