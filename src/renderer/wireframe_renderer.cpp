#include "renderer.h"
#include <cmath>
#include <cassert>

namespace renderer {
void Renderer::RenderTriangleWireframe(const RenderContext& ctx) const {
    auto draw_line = [&ctx](int x0, int y0, int x1, int y1, Color color) {
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
                if (y >= 0 && y < ctx.screen.GetWidth() && x >= 0 && x < ctx.screen.GetHeight()) {
                    ctx.screen.SetFrameBufferPixel(y, x, color.GetHex());
                }
            } else {
                if (x >= 0 && x < ctx.screen.GetWidth() && y >= 0 && y < ctx.screen.GetHeight()) {
                    ctx.screen.SetFrameBufferPixel(x, y, color.GetHex());
                }
            }

            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    };

    draw_line(ctx.point0.x(), ctx.point0.y(), ctx.point1.x(), ctx.point1.y(), ctx.color);
    draw_line(ctx.point1.x(), ctx.point1.y(), ctx.point2.x(), ctx.point2.y(), ctx.color);
    draw_line(ctx.point2.x(), ctx.point2.y(), ctx.point0.x(), ctx.point0.y(), ctx.color);
}
}  // namespace renderer
