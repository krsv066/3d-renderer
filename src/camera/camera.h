#pragma once

#include "linalg.h"
#include "screen.h"

namespace renderer {
static constexpr double kStraightAngle = 180.0;

struct Aspect {
    double value;
    explicit constexpr Aspect(double value) : value(value) {
    }
};

struct Fov {
    double value;
    explicit constexpr Fov(double value) : value(value * M_PI / kStraightAngle) {
    }
};

struct Near {
    double value;
    explicit constexpr Near(double value) : value(value) {
    }
};

struct Far {
    double value;
    explicit constexpr Far(double value) : value(value) {
    }
};

static constexpr Fov kDefaultFov = Fov{60.0};
static constexpr Near kDefaultNearBound = Near{0.1};
static constexpr Far kDefaultFarBound = Far{100.0};

class Camera {
public:
    Camera(Width width, Height height, Fov fov = kDefaultFov, Near near = kDefaultNearBound,
           Far far = kDefaultFarBound);
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
    const Aspect aspect_;
    const Fov fov_;
    const Near near_;
    const Far far_;

    void UpdateViewMatrix();
    void UpdateViewProjectionMatrix();
};
}  // namespace renderer
