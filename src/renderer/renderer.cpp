#include "renderer.h"
#include "object.h"
#include <cassert>

namespace renderer {
Renderer::Renderer(Mode mode) : render_mode_(mode) {
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

    switch (render_mode_) {
        case Mode::Wireframe:
            RenderTriangleWireframe(p0, p1, p2, triangle.color, screen);
            break;
        case Mode::Filled:
            RenderTriangleFilled(p0, p1, p2, triangle.color, screen);
            break;
    }
}

void Renderer::RenderTriangleWireframe(const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                       uint32_t color, Screen& screen) const {
    DrawLine(p0.x(), p0.y(), p1.x(), p1.y(), color, screen);
    DrawLine(p1.x(), p1.y(), p2.x(), p2.y(), color, screen);
    DrawLine(p2.x(), p2.y(), p0.x(), p0.y(), color, screen);
}

void Renderer::RenderTriangleFilled(const Vector4& p0, const Vector4& p1, const Vector4& p2,
                                    uint32_t color, Screen& screen) const {
    const int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x(), p1.x(), p2.x()}))));
    const int max_x = std::min(static_cast<int>(screen.GetWidth()) - 1,
                               static_cast<int>(std::ceil(std::max({p0.x(), p1.x(), p2.x()}))));
    const int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y(), p1.y(), p2.y()}))));
    const int max_y = std::min(static_cast<int>(screen.GetHeight()) - 1,
                               static_cast<int>(std::ceil(std::max({p0.y(), p1.y(), p2.y()}))));

    const double area = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            ProcessPixel(x, y, p0, p1, p2, area, color, screen);
        }
    }
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
                const int index = GetBufferIndex(y, x, screen) * 4;
                screen.SetFrameBufferPixel(index, color);
            }
        } else {
            if (x >= 0 && x < static_cast<int>(screen.GetWidth()) && y >= 0 &&
                y < static_cast<int>(screen.GetHeight())) {
                const int index = GetBufferIndex(x, y, screen) * 4;
                screen.SetFrameBufferPixel(index, color);
            }
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Renderer::ProcessPixel(int x, int y, const Vector4& p0, const Vector4& p1, const Vector4& p2,
                            double area, uint32_t color, Screen& screen) const {
    double w0 = EdgeFunction(p1.x(), p1.y(), p2.x(), p2.y(), x, y);
    double w1 = EdgeFunction(p2.x(), p2.y(), p0.x(), p0.y(), x, y);
    double w2 = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), x, y);

    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        UpdatePixel(x, y, w0 / area, w1 / area, w2 / area, p0, p1, p2, color, screen);
    }
}

void Renderer::UpdatePixel(int x, int y, double w0, double w1, double w2, const Vector4& p0,
                           const Vector4& p1, const Vector4& p2, uint32_t color,
                           Screen& screen) const {
    const double z = w0 * p0.z() + w1 * p1.z() + w2 * p2.z();
    const int index = GetBufferIndex(x, y, screen);

    if (z < screen.GetZBufferDepth(index)) {
        screen.SetZBufferDepth(index, z);
        const size_t pixel_index = index * 4;
        screen.SetFrameBufferPixel(pixel_index, color);
    }
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

int Renderer::GetBufferIndex(int x, int y, const Screen& screen) const {
    return (screen.GetHeight() - y) * screen.GetWidth() + x;
}

double Renderer::EdgeFunction(double x0, double y0, double x1, double y1, double x,
                              double y) const {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}
}  // namespace renderer
