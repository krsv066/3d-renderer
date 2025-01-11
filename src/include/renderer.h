#pragma once

#include "object.h"
#include <Eigen/Dense>
#include <cstdint>
#include <vector>

class Renderer {
public:
    Renderer() = delete;

    explicit Renderer(int width, int height);

    void SetProjectionMatrix(double fov, double aspect, double near, double far);

    void Rasterize(Object obj);

    void Show() const;

private:
    int width_;
    int height_;
    std::vector<double> z_buffer_;
    std::vector<uint32_t> frame_buffer_;
    Eigen::Matrix4d projection_matrix_;

    inline int GetBufferIndex(int x, int y) const;

    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x, double y) const;

    Eigen::Vector4d ProjectVertex(const Eigen::Vector3d& p) const;
};
