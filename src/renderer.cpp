#include "renderer.h"
#include "object.h"

Renderer::Renderer(int width, int height) : width_(width), height_(height) {
    z_buffer_.assign(width_ * height_, std::numeric_limits<double>::infinity());
    frame_buffer_.assign(width_ * height_, 0x000000);
}

void Renderer::Rasterize(Object obj) {
    for (const auto &tr : obj.obj_) {
        Point p0 = tr.a;
        Point p1 = tr.b;
        Point p2 = tr.c;
        uint32_t color = tr.color;

        int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x, p1.x, p2.x}))));
        int max_x = std::min(width_ - 1, static_cast<int>(std::ceil(std::max({p0.x, p1.x, p2.x}))));
        int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y, p1.y, p2.y}))));
        int max_y =
            std::min(height_ - 1, static_cast<int>(std::ceil(std::max({p0.y, p1.y, p2.y}))));

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
                    if (z < z_buffer_[index]) {
                        z_buffer_[index] = z;
                        frame_buffer_[index] = color;
                    }
                }
            }
        }
    }
}

void Renderer::Show() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int index = GetBufferIndex(x, y);
            uint32_t color = frame_buffer_[index];
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
}

int Renderer::GetBufferIndex(int x, int y) {
    return y * width_ + x;
}

double Renderer::EdgeFunction(double x0, double y0, double x1, double y1, double x, double y) {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}
