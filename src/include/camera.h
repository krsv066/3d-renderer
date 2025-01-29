#pragma once

#include <Eigen/Dense>

class Camera {
public:
    Camera(double fov, double aspect, double near, double far);

    const Eigen::Matrix4d& GetProjectionMatrix() const;

private:
    Eigen::Matrix4d projection_matrix_;
};
