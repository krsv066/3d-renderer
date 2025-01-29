#include "renderer.h"
#include "world.h"
#include "obj_parser.h"
#include <cmath>

int main() {
    Eigen::Vector3d translation(0.0, 0.0, -3.0);
    double angle = M_PI / 8.0;
    Eigen::Matrix3d rotation =
        Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitX()).toRotationMatrix() *
        Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitY()).toRotationMatrix();

    Object cube = ObjParser::LoadObj("../src/models/cube.obj", translation, rotation, 0xFF00FF);
    World scene{cube};

    Renderer renderer;
    renderer.SetRenderMode(RenderMode::WIREFRAME);
    uint32_t width = 1280;
    uint32_t height = 720;
    renderer.SetWindow(width, height);
    double fov = 60.0 * M_PI / 180.0;
    double aspect = static_cast<double>(width) / height;
    double near = 0.1;
    double far = 100.0;
    renderer.SetProjectionMatrix(fov, aspect, near, far);

    renderer.Render(scene);

    return 0;
}
