#include "camera.h"
#include <cmath>

Camera::Camera(double fov, double aspect, double near, double far) {
    const double tan_half_fov = std::tan(fov / 2.0);
    projection_matrix_ = Eigen::Matrix4d::Zero();
    projection_matrix_(0, 0) = 1.0 / (aspect * tan_half_fov);
    projection_matrix_(1, 1) = 1.0 / tan_half_fov;
    projection_matrix_(2, 2) = -(far + near) / (far - near);
    projection_matrix_(2, 3) = -(2.0 * far * near) / (far - near);
    projection_matrix_(3, 2) = -1.0;
}

const Eigen::Matrix4d& Camera::GetProjectionMatrix() const {
    return projection_matrix_;
}
