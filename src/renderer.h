#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

struct Point {
    double x, y, z;
};

class Renderer {
public:
    Renderer() = delete;

    explicit Renderer(int width, int height);

    void RasterizeTriangle(Point p0, Point p1, Point p2, uint32_t color);

    void Show();

private:
    int width_;
    int height_;
    std::vector<double> z_buffer_;
    std::vector<uint32_t> frame_buffer_;

    inline int GetBufferIndex(int x, int y);

    inline double EdgeFunction(double x0, double y0, double x1, double y1, double x, double y);
};
