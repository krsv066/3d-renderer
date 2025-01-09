#include "renderer.h"

int main(int argc, char* argv[]) {
    Renderer renderer(160, 40);

    Triangle t1 = {{20, 5, 0.5}, {80, 10, 0.3}, {40, 30, 0.7}, 0xFF0000};    // Red
    Triangle t2 = {{60, 5, 0.2}, {120, 20, 0.1}, {30, 25, 0.6}, 0x00FF00};   // Green
    Triangle t3 = {{150, 3, 0.4}, {120, 15, 0.2}, {100, 8, 0.4}, 0x0000FF};  // Blue
    Object object{t1, t2, t3};

    renderer.Rasterize(object);
    renderer.Show();

    return 0;
}
