#include "application.h"
#include "camera.h"
#include "obj_parser.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>

Application::Application()
    : renderer_(Renderer::Mode::Wireframe),
      world_{ObjParser::LoadObj(
          "../src/models/cube.obj", Eigen::Vector3d(0.0, 0.0, -3.0),
          Eigen::AngleAxisd(M_PI / 8.0, Eigen::Vector3d::UnitX()).toRotationMatrix() *
              Eigen::AngleAxisd(M_PI / 8.0, Eigen::Vector3d::UnitY()).toRotationMatrix(),
          0xFF00FF)},
      camera_(Fov{60.0 * M_PI / 180.0}, Aspect{1280.0 / 720.0}, Near{0.1}, Far{100.0}) {
}

void Application::Run() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Renderer");
    sf::Texture texture(sf::Vector2u(1280, 720));
    sf::Sprite sprite(texture);

    Screen screen(Width{1280}, Height{720});

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
