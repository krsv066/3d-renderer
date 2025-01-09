#include "renderer.h"

int main(int argc, char* argv[]) {
    Renderer renderer(160, 40);

    renderer.RasterizeTriangle({20, 5, 0.5}, {80, 10, 0.3}, {40, 30, 0.7}, 0xFF0000);    // Red
    renderer.RasterizeTriangle({60, 5, 0.2}, {120, 20, 0.1}, {30, 25, 0.6}, 0x00FF00);   // Green
    renderer.RasterizeTriangle({150, 3, 0.4}, {120, 15, 0.2}, {100, 8, 0.4}, 0x0000FF);  // Blue

    renderer.Show();

    return 0;
}
