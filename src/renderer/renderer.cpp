#include "renderer.h"
#include "object.h"
#include <cassert>

namespace renderer {
Renderer::Renderer(Mode render_mode) {
    switch (render_mode) {
        case Mode::Filled:
            render_triangle_ = [this](const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                      uint32_t color, Screen& screen) {
                this->RenderTriangleFilled(p0, p1, p2, color, screen);
            };
            break;
        case Mode::Wireframe:
            render_triangle_ = [this](const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                      uint32_t color, Screen& screen) {
                this->RenderTriangleWireframe(p0, p1, p2, color, screen);
            };
            break;
    }
}

Screen Renderer::Render(const World& scene, const Camera& camera, Screen&& screen) const {
    assert(screen.GetWidth() > 0 && screen.GetHeight() > 0);

    screen.Clear();
    for (const auto& obj : scene.GetObjects()) {
        for (const auto& triangle : obj.GetTriangles()) {
            RenderTriangle(obj, triangle, camera, screen);
        }
    }

    return std::move(screen);
}

void Renderer::RenderTriangle(const Object& obj, const primitive::Triangle& triangle,
                              const Camera& camera, Screen& screen) const {
    const Vector4 p0 = ProjectVertex(GetGlobalCoordinates(obj, triangle.a), camera, screen);
    const Vector4 p1 = ProjectVertex(GetGlobalCoordinates(obj, triangle.b), camera, screen);
    const Vector4 p2 = ProjectVertex(GetGlobalCoordinates(obj, triangle.c), camera, screen);

    render_triangle_(p0, p1, p2, triangle.color, screen);
}

Vector4 Renderer::ProjectVertex(const Vector3& p, const Camera& camera,
                                const Screen& screen) const {
    Vector4 pos(p.x(), p.y(), p.z(), 1.0);
    Vector4 projected = camera.GetProjectionMatrix() * pos;

    if (projected.w() != 0) {
        projected.x() /= projected.w();
        projected.y() /= projected.w();
        projected.z() /= projected.w();
    }

    projected.x() = (projected.x() + 1.0) * screen.GetWidth() * 0.5;
    projected.y() = (projected.y() + 1.0) * screen.GetHeight() * 0.5;

    return projected;
}

Vector3 Renderer::GetGlobalCoordinates(const Object& obj, const Vector3& p) const {
    return obj.GetRotation() * p + obj.GetTranslation();
}
}  // namespace renderer
