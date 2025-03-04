#pragma once

#include "camera.h"
#include "object.h"
#include "screen.h"
#include "world.h"
#include <Eigen/Dense>
#include <cstdint>

class Renderer {
public:
    enum class Mode : uint8_t { Filled, Wireframe };
    Renderer(Mode mode = Mode::Wireframe);
    Screen Render(const World& scene, const Camera& camera,
                  Screen&& screen = Screen(Width{0}, Height{0}));

private:
    Mode render_mode_;
    void RenderFrame(const World& scene, const Camera& camera, Screen& screen);
    void RenderObject(const Object& obj, const Camera& camera, Screen& screen);
    void RenderTriangle(const Object& obj, const Triangle& triangle, const Camera& camera,
                        Screen& screen);
    void RenderTriangleWireframe(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                                 const Eigen::Vector4d& p2, uint32_t color, Screen& screen);
    void RenderTriangleFilled(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                              const Eigen::Vector4d& p2, uint32_t color, Screen& screen);
    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color, Screen& screen);
    void ProcessPixel(int x, int y, const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                      const Eigen::Vector4d& p2, double area, uint32_t color, Screen& screen);
    void UpdatePixel(int x, int y, double w0, double w1, double w2, const Eigen::Vector4d& p0,
                     const Eigen::Vector4d& p1, const Eigen::Vector4d& p2, uint32_t color,
                     Screen& screen);
    Eigen::Vector4d ProjectVertex(const Eigen::Vector3d& p, const Camera& camera,
                                  const Screen& screen) const;
    inline Eigen::Vector3d GetGlobalCoordinates(const Object& obj, const Eigen::Vector3d& p) const;
    inline int GetBufferIndex(int x, int y, const Screen& screen) const;
    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x,
                               double y) const;
};
