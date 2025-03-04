#include "application.h"
#include "obj_parser.h"
#include "renderer.h"
#include "screen.h"
#include "world.h"

void Application::Run() {
    Eigen::Vector3d translation(0.0, 0.0, -3.0);
    double angle = M_PI / 8.0;
    Eigen::Matrix3d rotation =
        Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitX()).toRotationMatrix() *
        Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitY()).toRotationMatrix();

    Object cube = ObjParser::LoadObj("../src/models/cube.obj", translation, rotation, 0xFF00FF);
    World scene{cube};

    Screen screen(Width{1280}, Height{720});

    Camera camera(Fov{60.0 * M_PI / 180.0}, Aspect{static_cast<double>(1280) / 720}, Near{0.1},
                  Far{100.0});

    Renderer::Mode mode = Renderer::Mode::Wireframe;
    Renderer renderer(camera, scene, screen, mode);

    renderer.Render();
}
