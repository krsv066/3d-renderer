#pragma once

#include "camera.h"
#include "linalg.h"
#include "screen.h"
#include "world.h"
#include <cstdint>
#include <functional>

namespace renderer {
class Renderer {
public:
    enum class Mode : uint8_t { Filled, Wireframe };
    Renderer(Mode mode = Mode::Wireframe);
    Screen Render(const World& scene, const Camera& camera,
                  Screen&& screen = Screen(Width{0}, Height{0})) const;

private:
    using RenderTriangleFunc = std::function<void(const Vector4& point0, const Vector4& point1,
                                                  const Vector4& point2, uint32_t, Screen&)>;
    RenderTriangleFunc render_triangle_;

    void RenderTriangle(const Object& obj, const primitive::Triangle& triangle,
                        const Camera& camera, Screen& screen) const;
    Vector4 ProjectVertex(const Vector3& point, const Camera& camera, const Screen& screen) const;
    void RenderTriangleWireframe(const Vector4& point0, const Vector4& point1,
                                 const Vector4& point2, uint32_t color, Screen& screen) const;
    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color, Screen& screen) const;
    void RenderTriangleFilled(const Vector4& point0, const Vector4& point1, const Vector4& point2,
                              uint32_t color, Screen& screen) const;
    void ProcessPixel(int x, int y, const Vector4& point0, const Vector4& point1,
                      const Vector4& point2, double area, uint32_t color, Screen& screen) const;
    void UpdatePixel(int x, int y, double w0, double w1, double w2, const Vector4& point0,
                     const Vector4& point1, const Vector4& point2, uint32_t color,
                     Screen& screen) const;
    inline Vector3 GetGlobalCoordinates(const Object& obj, const Vector3& point) const;
    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x,
                               double y) const;
};
}  // namespace renderer
