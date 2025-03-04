#include "application.h"
#include "camera.h"
#include "obj_parser.h"
#include "renderer.h"

Application::Application()
    : scene_{ObjParser::LoadObj(
          "../src/models/cube.obj", Eigen::Vector3d(0.0, 0.0, -3.0),
          Eigen::AngleAxisd(M_PI / 8.0, Eigen::Vector3d::UnitX()).toRotationMatrix() *
              Eigen::AngleAxisd(M_PI / 8.0, Eigen::Vector3d::UnitY()).toRotationMatrix(),
          0xFF00FF)},
      camera_(Fov{60.0 * M_PI / 180.0}, Aspect{1280.0 / 720.0}, Near{0.1}, Far{100.0}),
      screen_(Width{1280}, Height{720}),
      renderer_(camera_, scene_, screen_, Renderer::Mode::Wireframe) {
}

void Application::Run() {
    renderer_.Render();
}
