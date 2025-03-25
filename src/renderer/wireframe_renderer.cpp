#include "renderer.h"
#include <cmath>
#include <cassert>

namespace renderer {
void Renderer::RenderTriangleWireframe(const linalg::Vector4& point0, const linalg::Vector4& point1,
                                       const linalg::Vector4& point2, uint32_t color,
                                       Screen& screen, const linalg::Vector3& normal,
                                       const std::vector<Light>& lights) const {
    auto draw_line = [&screen](int x0, int y0, int x1, int y1, uint32_t color) {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }

        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        assert(dx >= 0);

        int dy = y1 - y0;
        int derror2 = std::abs(dy) * 2;
        int error2 = 0;
        int y = y0;

        for (int x = x0; x <= x1; ++x) {
            if (steep) {
                if (y >= 0 && y < screen.GetWidth() && x >= 0 && x < screen.GetHeight()) {
                    screen.SetFrameBufferPixel(y, x, color);
                }
            } else {
                if (x >= 0 && x < screen.GetWidth() && y >= 0 && y < screen.GetHeight()) {
                    screen.SetFrameBufferPixel(x, y, color);
                }
            }

            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    };

    draw_line(point0.x(), point0.y(), point1.x(), point1.y(), color);
    draw_line(point1.x(), point1.y(), point2.x(), point2.y(), color);
    draw_line(point2.x(), point2.y(), point0.x(), point0.y(), color);
}
}  // namespace renderer
