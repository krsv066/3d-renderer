#pragma once

#include "linalg.h"

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
    const linalg::Matrix4& GetViewProjectionMatrix() const;
    void MoveForward(double distance);
    void MoveBackward(double distance);
    void MoveRight(double distance);
    void MoveLeft(double distance);
    void MoveUp(double distance);
    void MoveDown(double distance);
    void RotateHorizontal(double angle);
    void RotateVertical(double angle);
    void RotateRoll(double angle);

private:
    linalg::Matrix4 projection_matrix_;
    linalg::Matrix4 view_matrix_;
    linalg::Matrix4 view_projection_matrix_;
    linalg::Vector3 position_ = linalg::Vector3(0, 0, 0);
    linalg::Vector3 front_ = linalg::Vector3(0, 0, -1);
    linalg::Vector3 up_ = linalg::Vector3(0, 1, 0);
    linalg::Vector3 right_ = linalg::Vector3(1, 0, 0);
    Fov fov_;
    Aspect aspect_;
    Near near_;
    Far far_;

    void UpdateViewMatrix();
    void UpdateViewProjectionMatrix();
};
}  // namespace renderer
