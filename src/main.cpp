#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

struct Point {
    double x, y, z;
};

constexpr int kWidth = 160;
constexpr int kHeight = 40;

inline int GetBufferIndex(int x, int y) {
    return y * kWidth + x;
}

inline double EdgeFunction(double x0, double y0, double x1, double y1, double x, double y) {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
};

void RasterizeTriangle(Point p0, Point p1, Point p2, std::vector<double>& z_buffer,
                       std::vector<uint32_t>& frame_buffer, uint32_t color) {
    int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x, p1.x, p2.x}))));
    int max_x = std::min(kWidth - 1, static_cast<int>(std::ceil(std::max({p0.x, p1.x, p2.x}))));
    int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y, p1.y, p2.y}))));
    int max_y = std::min(kHeight - 1, static_cast<int>(std::ceil(std::max({p0.y, p1.y, p2.y}))));

    double area = EdgeFunction(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = EdgeFunction(p1.x, p1.y, p2.x, p2.y, x, y);
            double w1 = EdgeFunction(p2.x, p2.y, p0.x, p0.y, x, y);
            double w2 = EdgeFunction(p0.x, p0.y, p1.x, p1.y, x, y);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                double z = w0 * p0.z + w1 * p1.z + w2 * p2.z;

                int index = GetBufferIndex(x, y);
                if (z < z_buffer[index]) {
                    z_buffer[index] = z;
                    frame_buffer[index] = color;
                }
            }
        }
    }
}

int main() {
    std::vector<double> z_buffer(kWidth * kHeight, std::numeric_limits<double>::infinity());
    std::vector<uint32_t> frame_buffer(kWidth * kHeight, 0x000000);

    /* clang-format off */
    RasterizeTriangle({20, 5, 0.5}, {80, 10, 0.3}, {40, 30, 0.7}, z_buffer, frame_buffer, 0xFF0000);  // Red
    RasterizeTriangle({60, 5, 0.2}, {120, 20, 0.1}, {30, 25, 0.6}, z_buffer, frame_buffer, 0x00FF00);  // Green
    RasterizeTriangle({150, 3, 0.4}, {120, 15, 0.2}, {100, 8, 0.4}, z_buffer, frame_buffer, 0x0000FF);  // Blue
    /* clang-format on */

    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            int index = GetBufferIndex(x, y);
            uint32_t color = frame_buffer[index];
            if (color == 0x000000) {
                std::cout << ' ';
            } else if (color == 0xFF0000) {
                std::cout << "\033[1;31mR\033[0m";
            } else if (color == 0x00FF00) {
                std::cout << "\033[1;32mG\033[0m";
            } else if (color == 0x0000FF) {
                std::cout << "\033[1;34mB\033[0m";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
