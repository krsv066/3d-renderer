#include "renderer.h"
#include "world.h"
#include <cmath>

int main() {
    Triangle t1 = {{1.0, 1.0, -2.5}, {-0.5, 0.5, -2.5}, {0.5, -0.5, -2.5}, 0xFF0000};   // Red
    Triangle t2 = {{1.0, 1.0, -3.5}, {-0.5, 0.5, -4.5}, {-0.2, -0.4, -1.5}, 0x00FF00};  // Green
    Object obj1{t1, t2};
    Triangle t3 = {{-0.4, -1.0, -5.8}, {0.9, 0.0, -1.5}, {-0.8, -0.1, -1.5}, 0x0000FF};  // Blue
    Object obj2{t3};
    World scene{obj1, obj2};

    const uint32_t width = 1280;
    const uint32_t height = 720;
    Renderer renderer(width, height);
    
    double fov = 60.0 * M_PI / 180.0;
    double aspect = static_cast<double>(width) / height;
    double near = 0.1;
    double far = 100.0;
    renderer.SetProjectionMatrix(fov, aspect, near, far);

    renderer.Rasterize(scene);
    renderer.Show();

    return 0;
}
