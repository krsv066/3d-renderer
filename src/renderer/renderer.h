#pragma once

#include "camera.h"
#include "linalg.h"
#include "screen.h"
#include "world.h"
#include <cstdint>
#include <functional>

namespace renderer {
struct RenderContext {
    const linalg::Vector4& point0;
    const linalg::Vector4& point1;
    const linalg::Vector4& point2;
    Color color;
    Screen& screen;
    const linalg::Vector3& normal;
    const std::vector<Light>& lights;
};

class Renderer {
public:
    enum class Mode : uint8_t { Wireframe, Filled };
    Renderer(Mode mode = Mode::Wireframe);
    Screen Render(const World& scene, const Camera& camera, Screen&& screen) const;
    void SetMode(Mode mode);

private:
    using RenderTriangleFunc = std::function<void(const RenderContext& context)>;
    RenderTriangleFunc render_triangle_;

    void RenderTriangle(const Object& obj, const primitive::Triangle& triangle,
                        const Camera& camera, Screen& screen, const World& world) const;
    void RenderTriangleWireframe(const RenderContext& context) const;
    void RenderTriangleFilled(const RenderContext& context) const;
    linalg::Vector4 ProjectVertex(const linalg::Vector3& point, const Camera& camera,
                                  const Screen& screen) const;
    Color CalculateLighting(Color base_color, const linalg::Vector3& normal,
                            const std::vector<Light>& lights) const;

    inline linalg::Vector3 GetGlobalCoordinates(const Object& obj,
                                                const linalg::Vector3& point) const {
        return obj.GetRotation() * point + obj.GetTranslation();
    }

    inline linalg::Vector3 CalculateNormal(const linalg::Vector3& a, const linalg::Vector3& b,
                                           const linalg::Vector3& c) const {
        return (b - a).cross(c - a).normalized();
    }

    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x,
                               double y) const {
        return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
    }
};
}  // namespace renderer
