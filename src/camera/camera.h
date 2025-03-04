#pragma once

#include <Eigen/Dense>

namespace renderer {
struct Fov {
    double value;
    Fov(double value) : value(value) {
    }
};

struct Aspect {
    double value;
    explicit Aspect(double value) : value(value) {
    }
};

struct Near {
    double value;
    explicit Near(double value) : value(value) {
    }
};

struct Far {
    double value;
    explicit Far(double value) : value(value) {
    }
};

class Camera {
public:
    Camera(Fov fov, Aspect aspect, Near near, Far far);
    const Eigen::Matrix4d& GetProjectionMatrix() const;

private:
    Eigen::Matrix4d projection_matrix_;
};
}  // namespace renderer
