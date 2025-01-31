#pragma once

#include "camera.h"
#include "object.h"
#include "screen.h"
#include "world.h"
#include <Eigen/Dense>
#include <cstddef>
#include <cstdint>
#include <vector>

class Renderer {
public:
    enum class Mode : uint8_t { Filled, Wireframe };

    Renderer(const Camera& camera, const World& scene, const Screen& screen,
             Mode mod = Mode::Wireframe);

    void Render();

private:
    Camera camera_;
    World scene_;
    Screen screen_;
    std::vector<double> z_buffer_;
    std::vector<uint8_t> frame_buffer_;
    Mode render_mode_;

    void RenderFrame();

    void RenderObject(const Object& obj);

    void RenderTriangle(const Object& obj, const Triangle& triangle);

    void RenderTriangleWireframe(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                                 const Eigen::Vector4d& p2, uint32_t color);

    void RenderTriangleFilled(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                              const Eigen::Vector4d& p2, uint32_t color);

    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

    void ProcessPixel(int x, int y, const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                      const Eigen::Vector4d& p2, double area, uint32_t color);

    void UpdatePixel(int x, int y, double w0, double w1, double w2, const Eigen::Vector4d& p0,
                     const Eigen::Vector4d& p1, const Eigen::Vector4d& p2, uint32_t color);

    void Rasterize(const World& scene);

    void ShowFrame() const;

    Eigen::Vector4d ProjectVertex(const Eigen::Vector3d& p) const;

    inline Eigen::Vector3d GetGlobalCoordinates(const Object& obj, const Eigen::Vector3d& p) const;

    inline int GetBufferIndex(int x, int y) const;

    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x,
                               double y) const;
};
