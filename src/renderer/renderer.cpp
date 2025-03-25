#include "renderer.h"
#include "object.h"
#include <cassert>

namespace renderer {
Renderer::Renderer(Mode render_mode) {
    SetMode(render_mode);
}

void Renderer::SetMode(Mode render_mode) {
    switch (render_mode) {
        case Mode::Filled:
            render_triangle_ = [this](const RenderContext& context) {
                this->RenderTriangleFilled(context);
            };
            break;
        case Mode::Wireframe:
            render_triangle_ = [this](const RenderContext& context) {
                this->RenderTriangleWireframe(context);
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

    RenderContext context{point0, point1, point2,           triangle.color,
                          screen, normal, world.GetLights()};
    render_triangle_(context);
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
}  // namespace renderer
