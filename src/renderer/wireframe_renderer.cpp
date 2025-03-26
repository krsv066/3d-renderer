#include "renderer.h"
#include <cmath>

namespace renderer {
void Renderer::RenderTriangleWireframe(const TriangleProjected& triangle_pr,
                                       const std::vector<Light>&, Screen& screen) const {
    auto draw_line = [&](int x0, int y0, int x1, int y1, Color color) {
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
                if (y >= 0 && y < screen.GetWidth() && x >= 0 && x < screen.GetHeight()) {
                    screen.SetFrameBufferPixel(y, x, color.GetHex());
                }
            } else {
                if (x >= 0 && x < screen.GetWidth() && y >= 0 && y < screen.GetHeight()) {
                    screen.SetFrameBufferPixel(x, y, color.GetHex());
                }
            }

            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    };

    draw_line(triangle_pr.a.x(), triangle_pr.a.y(), triangle_pr.b.x(), triangle_pr.b.y(),
              triangle_pr.color);
    draw_line(triangle_pr.b.x(), triangle_pr.b.y(), triangle_pr.c.x(), triangle_pr.c.y(),
              triangle_pr.color);
    draw_line(triangle_pr.c.x(), triangle_pr.c.y(), triangle_pr.a.x(), triangle_pr.a.y(),
              triangle_pr.color);
}
}  // namespace renderer
