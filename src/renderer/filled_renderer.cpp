#include "renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace renderer {
void Renderer::RenderTriangleFilled(const linalg::Vector4& point0, const linalg::Vector4& point1,
                                    const linalg::Vector4& point2, uint32_t color, Screen& screen,
                                    const linalg::Vector3& normal,
                                    const std::vector<Light>& lights) const {
    const int min_x =
        std::max(0, static_cast<int>(std::floor(std::min({point0.x(), point1.x(), point2.x()}))));
    const int max_x =
        std::min(static_cast<int>(screen.GetWidth()) - 1,
                 static_cast<int>(std::ceil(std::max({point0.x(), point1.x(), point2.x()}))));
    const int min_y =
        std::max(0, static_cast<int>(std::floor(std::min({point0.y(), point1.y(), point2.y()}))));
    const int max_y =
        std::min(static_cast<int>(screen.GetHeight()) - 1,
                 static_cast<int>(std::ceil(std::max({point0.y(), point1.y(), point2.y()}))));

    const double area =
        EdgeFunction(point0.x(), point0.y(), point1.x(), point1.y(), point2.x(), point2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = EdgeFunction(point1.x(), point1.y(), point2.x(), point2.y(), x, y);
            double w1 = EdgeFunction(point2.x(), point2.y(), point0.x(), point0.y(), x, y);
            double w2 = EdgeFunction(point0.x(), point0.y(), point1.x(), point1.y(), x, y);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                const double z = w0 * point0.z() + w1 * point1.z() + w2 * point2.z();
                if (z < screen.GetZBufferDepth(x, y)) {
                    screen.SetZBufferDepth(x, y, z);

                    linalg::Vector3 position(w0 * point0.x() + w1 * point1.x() + w2 * point2.x(),
                                             w0 * point0.y() + w1 * point1.y() + w2 * point2.y(),
                                             z);

                    uint32_t lit_color = CalculateLighting(color, normal, lights);
                    screen.SetFrameBufferPixel(x, y, lit_color);
                }
            }
        }
    }
}

uint32_t Renderer::CalculateLighting(uint32_t base_color, const linalg::Vector3& normal,
                                     const std::vector<Light>& lights) const {
    uint8_t r = (base_color >> 16) & 0xFF;
    uint8_t g = (base_color >> 8) & 0xFF;
    uint8_t b = base_color & 0xFF;

    linalg::Vector3 base_color_vec(r / 255.0, g / 255.0, b / 255.0);
    linalg::Vector3 final_color(0.0, 0.0, 0.0);

    for (const auto& light : lights) {
        switch (light.type) {
            case Light::Type::Ambient: {
                final_color += light.color.cwiseProduct(base_color_vec) * light.intensity;
                break;
            }
            case Light::Type::Directional: {
                double diffuse = std::max(0.0, -light.direction.dot(normal));
                final_color += light.color.cwiseProduct(base_color_vec) * diffuse * light.intensity;
                break;
            }
            default:
                assert(false);
                break;
        }
    }

    final_color.x() = std::min(1.0, std::max(0.0, final_color.x()));
    final_color.y() = std::min(1.0, std::max(0.0, final_color.y()));
    final_color.z() = std::min(1.0, std::max(0.0, final_color.z()));

    r = static_cast<uint8_t>(final_color.x() * 255);
    g = static_cast<uint8_t>(final_color.y() * 255);
    b = static_cast<uint8_t>(final_color.z() * 255);

    return (r << 16) | (g << 8) | b;
}
}  // namespace renderer
