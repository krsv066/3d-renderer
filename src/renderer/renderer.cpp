#include "renderer.h"
#include <cassert>
#include <memory>
#include "linalg.h"
#include "object.h"
#include "screen.h"

namespace renderer {
Renderer::Renderer(Mode render_mode) {
    SetMode(render_mode);
}

void Renderer::SetMode(Mode render_mode) {
    switch (render_mode) {
        case Mode::Wireframe:
            render_triangle_ = [this](const TriangleProjected &triangle_pr, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) {
                RenderTriangleWireframe(triangle_pr, lights, screen);
            };
            break;
        case Mode::Filled:
            render_triangle_ = [this](const TriangleProjected &triangle_pr, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) { RenderTriangleFilled(triangle_pr, lights, screen); };
            break;
        default:
            assert(false);
            break;
    }
}

std::unique_ptr<Screen> Renderer::Render(const World &world, const Camera &camera, std::unique_ptr<Screen> screen) const {
    assert(screen->GetWidth() > 0 && screen->GetHeight() > 0);

    screen->ResetBuffers();
    for (const auto &obj : world.GetObjects()) {
        for (const auto &triangle : obj.GetTriangles()) {
            RenderTriangle(obj, triangle, camera, world, screen);
        }
    }

    return screen;
}

void Renderer::RenderTriangle(const Object &obj, const Triangle &triangle, const Camera &camera, const World &world, std::unique_ptr<Screen> &screen) const {
    const Vector3 global_a = GetGlobalCoordinates(obj, Vector3(triangle.vertices.col(0)));
    const Vector3 global_b = GetGlobalCoordinates(obj, Vector3(triangle.vertices.col(1)));
    const Vector3 global_c = GetGlobalCoordinates(obj, Vector3(triangle.vertices.col(2)));

    Vector4 pos_a(global_a.x(), global_a.y(), global_a.z(), 1.0);
    Vector4 pos_b(global_b.x(), global_b.y(), global_b.z(), 1.0);
    Vector4 pos_c(global_c.x(), global_c.y(), global_c.z(), 1.0);

    Vector4 clip_a = camera.GetViewProjectionMatrix() * pos_a;
    Vector4 clip_b = camera.GetViewProjectionMatrix() * pos_b;
    Vector4 clip_c = camera.GetViewProjectionMatrix() * pos_c;

    Vector3 normal = (global_b - global_a).cross(global_c - global_a).normalized();

    double near_plane_z = -camera.GetNear();

    std::vector<TriangleProjected> clipped_triangles = ClipTriangleAgainstNearPlane(clip_a, clip_b, clip_c, normal, triangle.color, near_plane_z);

    for (const auto &clipped : clipped_triangles) {
        TriangleProjected triangle_pr = ProjectTriangleToScreen(clipped, screen);
        render_triangle_(triangle_pr, world.GetLights(), screen);
    }
}

std::vector<TriangleProjected> Renderer::ClipTriangleAgainstNearPlane(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, const Vector3 &normal, const Color &color, double near_z) const {
    std::vector<TriangleProjected> result;

    bool v1_inside = v1.z() >= near_z;
    bool v2_inside = v2.z() >= near_z;
    bool v3_inside = v3.z() >= near_z;

    int vertices_behind = (!v1_inside ? 1 : 0) + (!v2_inside ? 1 : 0) + (!v3_inside ? 1 : 0);

    if (vertices_behind == 0) {
        Matrix43 verts;
        verts.col(0) = v1;
        verts.col(1) = v2;
        verts.col(2) = v3;
        result.emplace_back(verts, normal, color);
        return result;
    }

    if (vertices_behind == 3) {
        return result;
    }

    if (vertices_behind == 1) {
        if (!v1_inside) {
            Vector4 new_v1a = InterpolateVertex(v1, v2, near_z);
            Vector4 new_v1b = InterpolateVertex(v1, v3, near_z);

            Matrix43 verts1, verts2;
            verts1.col(0) = new_v1a;
            verts1.col(1) = v2;
            verts1.col(2) = v3;
            verts2.col(0) = new_v1a;
            verts2.col(1) = v3;
            verts2.col(2) = new_v1b;

            result.emplace_back(verts1, normal, color);
            result.emplace_back(verts2, normal, color);
        } else if (!v2_inside) {
            Vector4 new_v2a = InterpolateVertex(v2, v1, near_z);
            Vector4 new_v2b = InterpolateVertex(v2, v3, near_z);

            Matrix43 verts1, verts2;
            verts1.col(0) = v1;
            verts1.col(1) = new_v2a;
            verts1.col(2) = v3;
            verts2.col(0) = new_v2a;
            verts2.col(1) = new_v2b;
            verts2.col(2) = v3;

            result.emplace_back(verts1, normal, color);
            result.emplace_back(verts2, normal, color);
        } else {
            Vector4 new_v3a = InterpolateVertex(v3, v1, near_z);
            Vector4 new_v3b = InterpolateVertex(v3, v2, near_z);

            Matrix43 verts1, verts2;
            verts1.col(0) = v1;
            verts1.col(1) = v2;
            verts1.col(2) = new_v3b;
            verts2.col(0) = v1;
            verts2.col(1) = new_v3b;
            verts2.col(2) = new_v3a;

            result.emplace_back(verts1, normal, color);
            result.emplace_back(verts2, normal, color);
        }
    }

    if (vertices_behind == 2) {
        if (v1_inside) {
            Vector4 new_v2 = InterpolateVertex(v2, v1, near_z);
            Vector4 new_v3 = InterpolateVertex(v3, v1, near_z);

            Matrix43 verts;
            verts.col(0) = v1;
            verts.col(1) = new_v2;
            verts.col(2) = new_v3;

            result.emplace_back(verts, normal, color);
        } else if (v2_inside) {
            Vector4 new_v1 = InterpolateVertex(v1, v2, near_z);
            Vector4 new_v3 = InterpolateVertex(v3, v2, near_z);

            Matrix43 verts;
            verts.col(0) = new_v1;
            verts.col(1) = v2;
            verts.col(2) = new_v3;

            result.emplace_back(verts, normal, color);
        } else {
            Vector4 new_v1 = InterpolateVertex(v1, v3, near_z);
            Vector4 new_v2 = InterpolateVertex(v2, v3, near_z);

            Matrix43 verts;
            verts.col(0) = new_v1;
            verts.col(1) = new_v2;
            verts.col(2) = v3;

            result.emplace_back(verts, normal, color);
        }
    }

    return result;
}

TriangleProjected Renderer::ProjectTriangleToScreen(const TriangleProjected &clipped, std::unique_ptr<Screen> &screen) const {
    Matrix43 points = clipped.vertices;
    Vector4 point0 = points.col(0);
    Vector4 point1 = points.col(1);
    Vector4 point2 = points.col(2);

    double clip_z0 = point0.z();
    double clip_z1 = point1.z();
    double clip_z2 = point2.z();

    if (point0.w() != 0) {
        point0.x() /= point0.w();
        point0.y() /= point0.w();
        point0.z() /= point0.w();
    }
    if (point1.w() != 0) {
        point1.x() /= point1.w();
        point1.y() /= point1.w();
        point1.z() /= point1.w();
    }
    if (point2.w() != 0) {
        point2.x() /= point2.w();
        point2.y() /= point2.w();
        point2.z() /= point2.w();
    }

    point0.x() = (point0.x() + 1.0) * screen->GetWidth() * 0.5;
    point0.y() = (point0.y() + 1.0) * screen->GetHeight() * 0.5;
    point0.z() = clip_z0;

    point1.x() = (point1.x() + 1.0) * screen->GetWidth() * 0.5;
    point1.y() = (point1.y() + 1.0) * screen->GetHeight() * 0.5;
    point1.z() = clip_z1;

    point2.x() = (point2.x() + 1.0) * screen->GetWidth() * 0.5;
    point2.y() = (point2.y() + 1.0) * screen->GetHeight() * 0.5;
    point2.z() = clip_z2;

    Matrix43 transformed_verts;
    transformed_verts.col(0) = point0;
    transformed_verts.col(1) = point1;
    transformed_verts.col(2) = point2;

    return {transformed_verts, clipped.normal, clipped.color};
}

void Renderer::RenderTriangleWireframe(const TriangleProjected &triangle_pr, const std::vector<Light> &, std::unique_ptr<Screen> &screen) const {
    const Vector4 &a = triangle_pr.vertices.col(0);
    const Vector4 &b = triangle_pr.vertices.col(1);
    const Vector4 &c = triangle_pr.vertices.col(2);

    DrawLine(a.x(), a.y(), b.x(), b.y(), triangle_pr.color, screen);
    DrawLine(b.x(), b.y(), c.x(), c.y(), triangle_pr.color, screen);
    DrawLine(c.x(), c.y(), a.x(), a.y(), triangle_pr.color, screen);
}

void Renderer::RenderTriangleFilled(const TriangleProjected &triangle_pr, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) const {
    const Vector4 &a = triangle_pr.vertices.col(0);
    const Vector4 &b = triangle_pr.vertices.col(1);
    const Vector4 &c = triangle_pr.vertices.col(2);

    const double min_x_coord = std::min({a.x(), b.x(), c.x()});
    const double max_x_coord = std::max({a.x(), b.x(), c.x()});
    const double min_y_coord = std::min({a.y(), b.y(), c.y()});
    const double max_y_coord = std::max({a.y(), b.y(), c.y()});

    const int min_x = GetMinScreenX(min_x_coord);
    const int max_x = GetMaxScreenX(max_x_coord, screen);
    const int min_y = GetMinScreenY(min_y_coord);
    const int max_y = GetMaxScreenY(max_y_coord, screen);

    const double area = CalculateSignedArea(a.x(), a.y(), b.x(), b.y(), c.x(), c.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            double w0 = CalculateSignedArea(b.x(), b.y(), c.x(), c.y(), x, y);
            double w1 = CalculateSignedArea(c.x(), c.y(), a.x(), a.y(), x, y);
            double w2 = CalculateSignedArea(a.x(), a.y(), b.x(), b.y(), x, y);

            RasterizePixel(a, b, c, triangle_pr.normal, triangle_pr.color, x, y, w0, w1, w2, area, lights, screen);
        }
    }
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, const Color &color, std::unique_ptr<Screen> &screen) const {
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
            if (y >= 0 && y < screen->GetWidth() && x >= 0 && x < screen->GetHeight()) {
                screen->SetFrameBufferPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < screen->GetWidth() && y >= 0 && y < screen->GetHeight()) {
                screen->SetFrameBufferPixel(x, y, color);
            }
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Renderer::RasterizePixel(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector3 &normal, const Color &color, int x, int y, double w0, double w1, double w2, double area,
                              const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) const {
    if (PointInTriangle(w0, w1, w2)) {
        const double z = GetZCoordByArea(a, b, c, w0, w1, w2, area);
        if (z < screen->GetZBufferDepth(x, y)) {
            screen->SetZBufferDepth(x, y, z);
            Color lit_color = CalculateLighting(color, normal, lights);
            screen->SetFrameBufferPixel(x, y, lit_color);
        }
    }
}

Color Renderer::CalculateLighting(Color base_color, const Vector3 &normal, const std::vector<Light> &lights) const {
    Color final_color = Color::Black();

    for (const auto &light : lights) {
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

Vector4 Renderer::InterpolateVertex(const Vector4 &v1, const Vector4 &v2, double near_z) const {
    double t = (near_z - v1.z()) / (v2.z() - v1.z());

    return Vector4(v1.x() + t * (v2.x() - v1.x()), v1.y() + t * (v2.y() - v1.y()), near_z, v1.w() + t * (v2.w() - v1.w()));
}

double Renderer::GetZCoordByArea(const Vector4 &a, const Vector4 &b, const Vector4 &c, double w0, double w1, double w2, double area) const {
    w0 /= area;
    w1 /= area;
    w2 /= area;
    return w0 * a.z() + w1 * b.z() + w2 * c.z();
}

inline Vector3 Renderer::GetGlobalCoordinates(const Object &obj, const Vector3 &point) const {
    return obj.GetRotation() * point + obj.GetTranslation();
}

inline double Renderer::CalculateSignedArea(double x0, double y0, double x1, double y1, double x, double y) const {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}

inline int Renderer::GetMinScreenX(double x) const {
    return std::max(0, static_cast<int>(std::floor(x)));
}

inline int Renderer::GetMaxScreenX(double x, const std::unique_ptr<Screen> &screen) const {
    return std::min(static_cast<int>(screen->GetWidth()) - 1, static_cast<int>(std::ceil(x)));
}

inline int Renderer::GetMinScreenY(double y) const {
    return std::max(0, static_cast<int>(std::floor(y)));
}

inline int Renderer::GetMaxScreenY(double y, const std::unique_ptr<Screen> &screen) const {
    return std::min(static_cast<int>(screen->GetHeight()) - 1, static_cast<int>(std::ceil(y)));
}

inline bool Renderer::PointInTriangle(double w0, double w1, double w2) const {
    return w0 >= 0 && w1 >= 0 && w2 >= 0;
}
}  // namespace renderer
