#pragma once

#include <Eigen/Dense>
#include <cstdint>

struct Triangle {
    Eigen::Vector3d a;
    Eigen::Vector3d b;
    Eigen::Vector3d c;
    uint32_t color;
};
