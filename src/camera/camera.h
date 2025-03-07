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
    const Matrix4& GetViewProjectionMatrix() const;
    void MoveForward(double distance);
    void MoveBackward(double distance);
    void MoveRight(double distance);
    void MoveLeft(double distance);
    void MoveUp(double distance);
    void MoveDown(double distance);

private:
    void UpdateViewMatrix();
    void UpdateViewProjectionMatrix();

    Matrix4 projection_matrix_;
    Matrix4 view_matrix_;
    Matrix4 view_projection_matrix_;
    Vector3 position_ = Vector3(0, 0, 0);
    Vector3 front_ = Vector3(0, 0, -1);
    Vector3 up_ = Vector3(0, 1, 0);
    Vector3 right_ = Vector3(1, 0, 0);
    Fov fov_;
    Aspect aspect_;
    Near near_;
    Far far_;
};
}  // namespace renderer
