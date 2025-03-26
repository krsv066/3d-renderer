#include "color.h"
#include "renderer.h"
#include <algorithm>
#include <cassert>

namespace renderer {
void Renderer::RenderTriangleFilled(const RenderContext& ctx) const {
    const double min_x_coord = std::min({ctx.point0.x(), ctx.point1.x(), ctx.point2.x()});
    const double max_x_coord = std::max({ctx.point0.x(), ctx.point1.x(), ctx.point2.x()});
    const double min_y_coord = std::min({ctx.point0.y(), ctx.point1.y(), ctx.point2.y()});
    const double max_y_coord = std::max({ctx.point0.y(), ctx.point1.y(), ctx.point2.y()});

    const int min_x = GetMinScreenX(min_x_coord);
    const int max_x = GetMaxScreenX(max_x_coord, ctx.screen);
    const int min_y = GetMinScreenY(min_y_coord);
    const int max_y = GetMaxScreenY(max_y_coord, ctx.screen);

    const double area = CalculateEdgeValue(ctx.point0.x(), ctx.point0.y(), ctx.point1.x(),
                                           ctx.point1.y(), ctx.point2.x(), ctx.point2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = CalculateEdgeValue(ctx.point1.x(), ctx.point1.y(), ctx.point2.x(),
                                           ctx.point2.y(), x, y);
            double w1 = CalculateEdgeValue(ctx.point2.x(), ctx.point2.y(), ctx.point0.x(),
                                           ctx.point0.y(), x, y);
            double w2 = CalculateEdgeValue(ctx.point0.x(), ctx.point0.y(), ctx.point1.x(),
                                           ctx.point1.y(), x, y);

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
