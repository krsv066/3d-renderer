#pragma once

#include "object.h"
#include "world.h"
#include <Eigen/Dense>
#include <cstdint>
#include <vector>

enum class RenderMode { FILLED, WIREFRAME };

class Renderer {
public:
    Renderer();

    void SetWindow(uint32_t width, uint32_t height);

    void SetProjectionMatrix(double fov, double aspect, double near, double far);

    void Render(const World& scene);

    void SetRenderMode(RenderMode mode);

private:
    uint32_t width_;
    uint32_t height_;
    std::vector<double> z_buffer_;
    std::vector<uint8_t> frame_buffer_;
    Eigen::Matrix4d projection_matrix_;
    RenderMode render_mode_ = RenderMode::FILLED;

    void RenderFrame(const World& scene);

    void RenderObject(const Object& obj);

    void RenderTriangle(const Object& obj, const Triangle& triangle);

    void RenderTriangleWireframe(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
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
