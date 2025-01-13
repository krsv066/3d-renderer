#include "renderer.h"
#include <cmath>

int main() {
    const uint32_t width = 1280;
    const uint32_t height = 720;
    Renderer renderer(width, height);

    double fov = 60.0 * M_PI / 180.0;
    double aspect = static_cast<double>(width) / height;
    double near = 0.1;
    double far = 100.0;
    renderer.SetProjectionMatrix(fov, aspect, near, far);

    Triangle t1 = {{1.0, 1.0, -2.5}, {-0.5, 0.5, -2.5}, {0.5, -0.5, -2.5}, 0xFF0000};    // Red
    Triangle t2 = {{1.0, 1.0, -3.5}, {-0.5, 0.5, -4.5}, {-0.2, -0.4, -1.5}, 0x00FF00};   // Green
    Triangle t3 = {{-0.4, -1.0, -5.8}, {0.9, 0.0, -1.5}, {-0.8, -0.1, -1.5}, 0x0000FF};  // Blue
    Object object{t1, t2, t3};

    renderer.Rasterize(object);
    renderer.Show();

    return 0;
}
