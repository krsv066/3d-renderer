#pragma once

#include <Eigen/Dense>

namespace renderer {
using AngleAxis = Eigen::AngleAxisd;
using Vector3 = Eigen::Vector3d;
using Vector4 = Eigen::Vector4d;
using Matrix3 = Eigen::Matrix3d;
using Matrix4 = Eigen::Matrix4d;
using Matrix43 = Eigen::Matrix<double, 4, 3>;
}  // namespace renderer
