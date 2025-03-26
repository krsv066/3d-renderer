#pragma once

#include "camera.h"
#include "linalg.h"
#include "screen.h"
#include "world.h"
#include <cmath>
#include <cstdint>
#include <functional>

namespace renderer {
struct RenderContext {
    const Vector4& point0;
    const Vector4& point1;
    const Vector4& point2;
    Color color;
    Screen& screen;
    const Vector3& normal;
    const std::vector<Light>& lights;
};

class Renderer {
public:
    enum class Mode : uint8_t { Wireframe, Filled };
    Renderer(Mode mode = Mode::Wireframe);
    Screen Render(const World& scene, const Camera& camera, Screen&& screen) const;
    void SetMode(Mode mode);

private:
    using RenderTriangleFunc = std::function<void(const RenderContext& ctx)>;
    RenderTriangleFunc render_triangle_;

    void RenderTriangle(const Object& obj, const Triangle& triangle, const Camera& camera,
                        Screen& screen, const World& world) const;
    void RenderTriangleWireframe(const RenderContext& ctx) const;
    void RenderTriangleFilled(const RenderContext& ctx) const;
    Vector4 ProjectVertex(const Vector3& point, const Camera& camera, const Screen& screen) const;
    Color CalculateLighting(Color base_color, const Vector3& normal,
                            const std::vector<Light>& lights) const;

    inline Vector3 GetGlobalCoordinates(const Object& obj, const Vector3& point) const {
        return obj.GetRotation() * point + obj.GetTranslation();
    }

    inline Vector3 CalculateNormal(const Vector3& a, const Vector3& b, const Vector3& c) const {
        return (b - a).cross(c - a).normalized();
    }

    inline double CalculateEdgeValue(double x0, double y0, double x1, double y1, double x,
                                     double y) const {
        return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
    }

    inline int ToScreenX(double x, const Screen& screen) const {
        return std::min(static_cast<int>(std::ceil(x)), static_cast<int>(screen.GetWidth()) - 1);
    }

    inline int ToScreenY(double y, const Screen& screen) const {
        return std::min(static_cast<int>(std::ceil(y)), static_cast<int>(screen.GetHeight()) - 1);
    }

    inline int GetMinScreenX(double x) const {
        return std::max(0, static_cast<int>(std::floor(x)));
    }

    inline int GetMaxScreenX(double x, const Screen& screen) const {
        return std::min(static_cast<int>(screen.GetWidth()) - 1, static_cast<int>(std::ceil(x)));
    }

    inline int GetMinScreenY(double y) const {
        return std::max(0, static_cast<int>(std::floor(y)));
    }

    inline int GetMaxScreenY(double y, const Screen& screen) const {
        return std::min(static_cast<int>(screen.GetHeight()) - 1, static_cast<int>(std::ceil(y)));
    }
};
}  // namespace renderer
