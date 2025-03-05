#include "renderer.h"
#include <cmath>

namespace renderer {
void Renderer::RenderTriangleWireframe(const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                       uint32_t color, Screen& screen) const {
    DrawLine(p0.x(), p0.y(), p1.x(), p1.y(), color, screen);
    DrawLine(p1.x(), p1.y(), p2.x(), p2.y(), color, screen);
    DrawLine(p2.x(), p2.y(), p0.x(), p0.y(), color, screen);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, uint32_t color, Screen& screen) const {
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
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        if (steep) {
            if (y >= 0 && y < static_cast<int>(screen.GetWidth()) && x >= 0 &&
                x < static_cast<int>(screen.GetHeight())) {
                screen.SetFrameBufferPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < static_cast<int>(screen.GetWidth()) && y >= 0 &&
                y < static_cast<int>(screen.GetHeight())) {
                screen.SetFrameBufferPixel(x, y, color);
            }
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}
}  // namespace renderer
