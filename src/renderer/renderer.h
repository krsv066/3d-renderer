#pragma once

#include "camera.h"
#include "linalg.h"
#include "screen.h"
#include "world.h"
#include <cstdint>
#include <functional>
#include <memory>

namespace renderer {
class Renderer {
public:
    enum class Mode : uint8_t { Wireframe, Filled };

    Renderer(Mode mode = Mode::Wireframe);
    void SetMode(Mode mode);
    std::unique_ptr<Screen> Render(const World &scene, const Camera &camera, std::unique_ptr<Screen> screen) const;
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

private:
    using RenderTriangleFunc = std::function<void(const TriangleProjected &triangle_proj, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen)>;

    void RenderTriangle(const Object &obj, const Triangle &triangle, const Camera &camera, const World &world, std::unique_ptr<Screen> &screen) const;
    std::vector<TriangleProjected> ClipTriangleAgainstNearPlane(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, const Vector3 &normal, const Color &color, double near_z) const;
    TriangleProjected ProjectTriangleToScreen(const TriangleProjected &clipped, std::unique_ptr<Screen> &screen) const;
    void RenderTriangleWireframe(const TriangleProjected &triangle_pr, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) const;
    void RenderTriangleFilled(const TriangleProjected &triangle_pr, const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) const;
    void DrawLine(int x0, int y0, int x1, int y1, const Color &color, std::unique_ptr<Screen> &screen) const;
    void RasterizePixel(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector3 &normal, const Color &color, int x, int y, double w0, double w1, double w2, double area,
                        const std::vector<Light> &lights, std::unique_ptr<Screen> &screen) const;
    Color CalculateLighting(Color base_color, const Vector3 &normal, const std::vector<Light> &lights) const;
    Vector4 InterpolateVertex(const Vector4 &v1, const Vector4 &v2, double near_z) const;
    Vector3 GetGlobalCoordinates(const Object &obj, const Vector3 &point) const;
    double GetZCoordByArea(const Vector4 &a, const Vector4 &b, const Vector4 &c, double w0, double w1, double w2, double area) const;
    double CalculateSignedArea(double x0, double y0, double x1, double y1, double x, double y) const;
    int GetMinScreenX(double x) const;
    int GetMaxScreenX(double x, const std::unique_ptr<Screen> &screen) const;
    int GetMinScreenY(double y) const;
    int GetMaxScreenY(double y, const std::unique_ptr<Screen> &screen) const;
    bool PointInTriangle(double w0, double w1, double w2) const;

    RenderTriangleFunc render_triangle_;
};
}  // namespace renderer
