#include "color.h"
#include "renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace renderer {
void Renderer::RenderTriangleFilled(const RenderContext& ctx) const {
    const int min_x = std::max(0, static_cast<int>(std::floor(std::min({ctx.point0.x(), ctx.point1.x(), ctx.point2.x()}))));
    const int max_x = std::min(static_cast<int>(ctx.screen.GetWidth()) - 1, static_cast<int>(std::ceil(std::max({ctx.point0.x(), ctx.point1.x(), ctx.point2.x()}))));
    const int min_y = std::max(0, static_cast<int>(std::floor(std::min({ctx.point0.y(), ctx.point1.y(), ctx.point2.y()}))));
    const int max_y = std::min(static_cast<int>(ctx.screen.GetHeight()) - 1, static_cast<int>(std::ceil(std::max({ctx.point0.y(), ctx.point1.y(), ctx.point2.y()}))));

    const double area = CalculateEdgeValue(ctx.point0.x(), ctx.point0.y(), ctx.point1.x(), ctx.point1.y(), ctx.point2.x(), ctx.point2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = CalculateEdgeValue(ctx.point1.x(), ctx.point1.y(), ctx.point2.x(), ctx.point2.y(), x, y);
            double w1 = CalculateEdgeValue(ctx.point2.x(), ctx.point2.y(), ctx.point0.x(), ctx.point0.y(), x, y);
            double w2 = CalculateEdgeValue(ctx.point0.x(), ctx.point0.y(), ctx.point1.x(), ctx.point1.y(), x, y);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                const double z = w0 * ctx.point0.z() + w1 * ctx.point1.z() + w2 * ctx.point2.z();
                if (z < ctx.screen.GetZBufferDepth(x, y)) {
                    ctx.screen.SetZBufferDepth(x, y, z);
                    Color lit_color = CalculateLighting(ctx.color, ctx.normal, ctx.lights);
                    ctx.screen.SetFrameBufferPixel(x, y, lit_color.GetHex());
                }
            }
        }
    }
}

Color Renderer::CalculateLighting(Color base_color, const linalg::Vector3& normal,
                                  const std::vector<Light>& lights) const {
    Color final_color = kBlackColor;

    for (const auto& light : lights) {
        switch (light.type) {
            case Light::Type::Ambient: {
                final_color += light.color * base_color * light.intensity;
                break;
            }
            case Light::Type::Directional: {
                double diffuse = std::max(0.0, -light.direction.dot(normal));
                final_color += light.color * base_color * diffuse * light.intensity;
                break;
            }
            default:
                assert(false);
                break;
        }
    }

    return final_color;
}
}  // namespace renderer
