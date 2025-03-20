#include "renderer.h"
#include "object.h"
#include <cassert>

namespace renderer {
Renderer::Renderer(Mode render_mode) {
    switch (render_mode) {
        case Mode::Filled:
            render_triangle_ = [this](const linalg::Vector4& point0, const linalg::Vector4& point1,
                                      const linalg::Vector4& point2, uint32_t color, Screen& screen,
                                      const linalg::Vector3& normal,
                                      const std::vector<Light>& lights) {
                this->RenderTriangleFilled(point0, point1, point2, color, screen, normal, lights);
            };
            break;
        case Mode::Wireframe:
            render_triangle_ = [this](const linalg::Vector4& point0, const linalg::Vector4& point1,
                                      const linalg::Vector4& point2, uint32_t color, Screen& screen,
                                      const linalg::Vector3& normal,
                                      const std::vector<Light>& lights) {
                this->RenderTriangleWireframe(point0, point1, point2, color, screen, normal,
                                              lights);
            };
            break;
        default:
            assert(false);
            break;
    }
}

Screen Renderer::Render(const World& world, const Camera& camera, Screen&& screen) const {
    assert(screen.GetWidth() > 0 && screen.GetHeight() > 0);

    screen.Clear();
    for (const auto& obj : world.GetObjects()) {
        for (const auto& triangle : obj.GetTriangles()) {
            RenderTriangle(obj, triangle, camera, screen, world);
        }
    }

    return std::move(screen);
}

void Renderer::RenderTriangle(const Object& obj, const primitive::Triangle& triangle,
                              const Camera& camera, Screen& screen, const World& world) const {
    assert(render_triangle_ != nullptr);

    const linalg::Vector3 global_a = GetGlobalCoordinates(obj, triangle.a);
    const linalg::Vector3 global_b = GetGlobalCoordinates(obj, triangle.b);
    const linalg::Vector3 global_c = GetGlobalCoordinates(obj, triangle.c);

    const linalg::Vector4 point0 = ProjectVertex(global_a, camera, screen);
    const linalg::Vector4 point1 = ProjectVertex(global_b, camera, screen);
    const linalg::Vector4 point2 = ProjectVertex(global_c, camera, screen);

    if (point0.z() < 0.1 && point1.z() < 0.1 && point2.z() < 0.1) {
        return;
    }

    linalg::Vector3 normal = CalculateNormal(global_a, global_b, global_c);

    render_triangle_(point0, point1, point2, triangle.color, screen, normal, world.GetLights());
}

linalg::Vector3 Renderer::CalculateNormal(const linalg::Vector3& a, const linalg::Vector3& b,
                                          const linalg::Vector3& c) const {
    return (b - a).cross(c - a).normalized();
}

linalg::Vector4 Renderer::ProjectVertex(const linalg::Vector3& point, const Camera& camera,
                                        const Screen& screen) const {
    linalg::Vector4 pos(point.x(), point.y(), point.z(), 1.0);
    linalg::Vector4 projected = camera.GetViewProjectionMatrix() * pos;

    double clip_z = projected.z();
    if (projected.w() != 0) {
        projected.x() /= projected.w();
        projected.y() /= projected.w();
        projected.z() /= projected.w();
    }

    projected.x() = (projected.x() + 1.0) * screen.GetWidth() * 0.5;
    projected.y() = (projected.y() + 1.0) * screen.GetHeight() * 0.5;
    projected.z() = clip_z;

    return projected;
}

inline linalg::Vector3 Renderer::GetGlobalCoordinates(const Object& obj,
                                                      const linalg::Vector3& point) const {
    return obj.GetRotation() * point + obj.GetTranslation();
}

uint32_t Renderer::CalculateLighting(uint32_t base_color, const linalg::Vector3& position,
                                     const linalg::Vector3& normal,
                                     const std::vector<Light>& lights,
                                     const linalg::Vector3& camera_pos) const {
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
            case Light::Type::Point: {
                linalg::Vector3 light_dir = (position - light.position).normalized();
                double distance = (position - light.position).norm();
                double attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
                double diffuse = std::max(0.0, -light_dir.dot(normal));
                final_color += light.color.cwiseProduct(base_color_vec) * diffuse *
                               light.intensity * attenuation;
                break;
            }
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
