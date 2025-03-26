#include "color.h"
#include "renderer.h"
#include <algorithm>
#include <cassert>

namespace renderer {
void Renderer::RenderTriangleFilled(const TriangleProjected& triangle_pr,
                                    const std::vector<Light>& lights, Screen& screen) const {
    const double min_x_coord = std::min({triangle_pr.a.x(), triangle_pr.b.x(), triangle_pr.c.x()});
    const double max_x_coord = std::max({triangle_pr.a.x(), triangle_pr.b.x(), triangle_pr.c.x()});
    const double min_y_coord = std::min({triangle_pr.a.y(), triangle_pr.b.y(), triangle_pr.c.y()});
    const double max_y_coord = std::max({triangle_pr.a.y(), triangle_pr.b.y(), triangle_pr.c.y()});

    const int min_x = GetMinScreenX(min_x_coord);
    const int max_x = GetMaxScreenX(max_x_coord, screen);
    const int min_y = GetMinScreenY(min_y_coord);
    const int max_y = GetMaxScreenY(max_y_coord, screen);

    const double area = CalculateEdgeValue(triangle_pr.a.x(), triangle_pr.a.y(), triangle_pr.b.x(),
                                           triangle_pr.b.y(), triangle_pr.c.x(), triangle_pr.c.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = CalculateEdgeValue(triangle_pr.b.x(), triangle_pr.b.y(), triangle_pr.c.x(),
                                           triangle_pr.c.y(), x, y);
            double w1 = CalculateEdgeValue(triangle_pr.c.x(), triangle_pr.c.y(), triangle_pr.a.x(),
                                           triangle_pr.a.y(), x, y);
            double w2 = CalculateEdgeValue(triangle_pr.a.x(), triangle_pr.a.y(), triangle_pr.b.x(),
                                           triangle_pr.b.y(), x, y);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                const double z =
                    w0 * triangle_pr.a.z() + w1 * triangle_pr.b.z() + w2 * triangle_pr.c.z();
                if (z < screen.GetZBufferDepth(x, y)) {
                    screen.SetZBufferDepth(x, y, z);
                    Color lit_color =
                        CalculateLighting(triangle_pr.color, triangle_pr.normal, lights);
                    screen.SetFrameBufferPixel(x, y, lit_color.GetHex());
                }
            }
        }
    }
}

Color Renderer::CalculateLighting(Color base_color, const Vector3& normal,
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
