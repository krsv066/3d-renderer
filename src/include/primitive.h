#pragma once

#include <Eigen/Dense>
#include <cstdint>

struct Triangle {
    Eigen::Vector3d a, b, c;
    uint32_t color;
};
