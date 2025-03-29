#include "renderer.h"
#include "linalg.h"
#include "object.h"
#include <cassert>

namespace renderer {
Renderer::Renderer(Mode render_mode) {
    SetMode(render_mode);
}

void Renderer::SetMode(Mode render_mode) {
    switch (render_mode) {
        case Mode::Wireframe:
            render_triangle_ = [this](const TriangleProjected& triangle_pr,
                                      const std::vector<Light>& lights, Screen& screen) {
                RenderTriangleWireframe(triangle_pr, lights, screen);
            };
            break;
        case Mode::Filled:
            render_triangle_ = [this](const TriangleProjected& triangle_pr,
                                      const std::vector<Light>& lights, Screen& screen) {
                RenderTriangleFilled(triangle_pr, lights, screen);
            };
            break;
        default:
            assert(false);
            break;
    }
}

Screen Renderer::Render(const World& world, const Camera& camera, Screen&& screen) const {
    assert(screen.GetWidth() > 0 && screen.GetHeight() > 0);

    screen.FillBlackColor();
    for (const auto& obj : world.GetObjects()) {
        for (const auto& triangle : obj.GetTriangles()) {
            RenderTriangle(obj, triangle, camera, screen, world);
        }
    }

    return std::move(screen);
}

void Renderer::RenderTriangle(const Object& obj, const Triangle& triangle, const Camera& camera,
                              Screen& screen, const World& world) const {
    const Vector3 global_a = GetGlobalCoordinates(obj, triangle.a);
    const Vector3 global_b = GetGlobalCoordinates(obj, triangle.b);
    const Vector3 global_c = GetGlobalCoordinates(obj, triangle.c);

    Vector4 pos_a(global_a.x(), global_a.y(), global_a.z(), 1.0);
    Vector4 pos_b(global_b.x(), global_b.y(), global_b.z(), 1.0);
    Vector4 pos_c(global_c.x(), global_c.y(), global_c.z(), 1.0);

    Vector4 clip_a = camera.GetViewProjectionMatrix() * pos_a;
    Vector4 clip_b = camera.GetViewProjectionMatrix() * pos_b;
    Vector4 clip_c = camera.GetViewProjectionMatrix() * pos_c;

    Vector3 normal = (global_b - global_a).cross(global_c - global_a).normalized();

    double near_plane_z = -0.1;

    std::vector<TriangleProjected> clipped_triangles =
        ClipTriangleAgainstNearPlane(clip_a, clip_b, clip_c, normal, triangle.color, near_plane_z);

    for (const auto& clipped : clipped_triangles) {
        Vector4 point0 = clipped.a;
        Vector4 point1 = clipped.b;
        Vector4 point2 = clipped.c;

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

        point0.x() = (point0.x() + 1.0) * screen.GetWidth() * 0.5;
        point0.y() = (point0.y() + 1.0) * screen.GetHeight() * 0.5;
        point0.z() = clip_z0;

        point1.x() = (point1.x() + 1.0) * screen.GetWidth() * 0.5;
        point1.y() = (point1.y() + 1.0) * screen.GetHeight() * 0.5;
        point1.z() = clip_z1;

        point2.x() = (point2.x() + 1.0) * screen.GetWidth() * 0.5;
        point2.y() = (point2.y() + 1.0) * screen.GetHeight() * 0.5;
        point2.z() = clip_z2;

        TriangleProjected triangle_pr = {point0, point1, point2, normal, triangle.color};
        render_triangle_(triangle_pr, world.GetLights(), screen);
    }
}

Vector4 Renderer::InterpolateVertex(const Vector4& v1, const Vector4& v2, double near_z) const {
    double t = (near_z - v1.z()) / (v2.z() - v1.z());

    return Vector4(v1.x() + t * (v2.x() - v1.x()), v1.y() + t * (v2.y() - v1.y()), near_z,
                   v1.w() + t * (v2.w() - v1.w()));
}

std::vector<TriangleProjected> Renderer::ClipTriangleAgainstNearPlane(
    const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector3& normal,
    const Color& color, double near_z) const {

    std::vector<TriangleProjected> result;

    bool v1_inside = v1.z() >= near_z;
    bool v2_inside = v2.z() >= near_z;
    bool v3_inside = v3.z() >= near_z;

    int vertices_behind = (!v1_inside ? 1 : 0) + (!v2_inside ? 1 : 0) + (!v3_inside ? 1 : 0);

    if (vertices_behind == 0) {
        result.push_back({v1, v2, v3, normal, color});
        return result;
    }

    if (vertices_behind == 3) {
        return result;
    }

    if (vertices_behind == 1) {
        if (!v1_inside) {
            Vector4 new_v1a = InterpolateVertex(v1, v2, near_z);
            Vector4 new_v1b = InterpolateVertex(v1, v3, near_z);
            result.push_back({new_v1a, v2, v3, normal, color});
            result.push_back({new_v1a, v3, new_v1b, normal, color});
        } else if (!v2_inside) {
            Vector4 new_v2a = InterpolateVertex(v2, v1, near_z);
            Vector4 new_v2b = InterpolateVertex(v2, v3, near_z);
            result.push_back({v1, new_v2a, v3, normal, color});
            result.push_back({new_v2a, new_v2b, v3, normal, color});
        } else {
            Vector4 new_v3a = InterpolateVertex(v3, v1, near_z);
            Vector4 new_v3b = InterpolateVertex(v3, v2, near_z);
            result.push_back({v1, v2, new_v3b, normal, color});
            result.push_back({v1, new_v3b, new_v3a, normal, color});
        }
    }

    if (vertices_behind == 2) {
        if (v1_inside) {
            Vector4 new_v2 = InterpolateVertex(v2, v1, near_z);
            Vector4 new_v3 = InterpolateVertex(v3, v1, near_z);
            result.push_back({v1, new_v2, new_v3, normal, color});
        } else if (v2_inside) {
            Vector4 new_v1 = InterpolateVertex(v1, v2, near_z);
            Vector4 new_v3 = InterpolateVertex(v3, v2, near_z);
            result.push_back({new_v1, v2, new_v3, normal, color});
        } else {
            Vector4 new_v1 = InterpolateVertex(v1, v3, near_z);
            Vector4 new_v2 = InterpolateVertex(v2, v3, near_z);
            result.push_back({new_v1, new_v2, v3, normal, color});
        }
    }

    return result;
}

Vector4 Renderer::ProjectVertex(const Vector3& point, const Camera& camera,
                                const Screen& screen) const {
    Vector4 pos(point.x(), point.y(), point.z(), 1.0);
    Vector4 projected = camera.GetViewProjectionMatrix() * pos;

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
