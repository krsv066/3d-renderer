#include "renderer.h"
#include <algorithm>
#include <cmath>

namespace renderer {
void Renderer::RenderTriangleFilled(const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                    uint32_t color, Screen& screen) const {
    const int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x(), p1.x(), p2.x()}))));
    const int max_x = std::min(static_cast<int>(screen.GetWidth()) - 1,
                               static_cast<int>(std::ceil(std::max({p0.x(), p1.x(), p2.x()}))));
    const int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y(), p1.y(), p2.y()}))));
    const int max_y = std::min(static_cast<int>(screen.GetHeight()) - 1,
                               static_cast<int>(std::ceil(std::max({p0.y(), p1.y(), p2.y()}))));

    const double area = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            ProcessPixel(x, y, p0, p1, p2, area, color, screen);
        }
    }
}

void Renderer::ProcessPixel(int x, int y, const Vector4& p0, const Vector4& p1, const Vector4& p2,
                            double area, uint32_t color, Screen& screen) const {
    double w0 = EdgeFunction(p1.x(), p1.y(), p2.x(), p2.y(), x, y);
    double w1 = EdgeFunction(p2.x(), p2.y(), p0.x(), p0.y(), x, y);
    double w2 = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), x, y);

    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        UpdatePixel(x, y, w0 / area, w1 / area, w2 / area, p0, p1, p2, color, screen);
    }
}

void Renderer::UpdatePixel(int x, int y, double w0, double w1, double w2, const Vector4& p0,
                           const Vector4& p1, const Vector4& p2, uint32_t color,
                           Screen& screen) const {
    const double z = w0 * p0.z() + w1 * p1.z() + w2 * p2.z();
    if (z < screen.GetZBufferDepth(x, y)) {
        screen.SetZBufferDepth(x, y, z);
        screen.SetFrameBufferPixel(x, y, color);
    }
}

inline double Renderer::EdgeFunction(double x0, double y0, double x1, double y1, double x,
                                     double y) const {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}
}  // namespace renderer
