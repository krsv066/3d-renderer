#pragma once

#include "linalg.h"
#include "screen.h"

namespace renderer {
class Aspect {
public:
    explicit Aspect(double value);
    double Get() const;

private:
    double value_;
};

class Fov {
public:
    explicit Fov(double value);
    double Get() const;

private:
    double value_;
};

class Near {
public:
    explicit Near(double value);
    double Get() const;

private:
    double value_;
};

class Far {
public:
    explicit Far(double value);
    double Get() const;

private:
    double value_;
};

struct Movement {
    double forward_movement = 0.0;
    double right_movement = 0.0;
    double up_movement = 0.0;
    double horizontal_rotation = 0.0;
    double vertical_rotation = 0.0;
    double roll_rotation = 0.0;
};

class Camera {
public:
    Camera(Width width, Height height, Fov fov = Fov{60.0}, Near near = Near{0.1},
           Far far = Far{100.0});
    const Matrix4& GetViewProjectionMatrix() const;
    void Move(const Movement& movement);
    double GetNear() const;

private:
    static Matrix4 MakeProjectionMatrix(Width width, Height height, Fov fov, Near near, Far far);
    static Matrix4 MakeViewMatrix(const Vector3& position, const Vector3& front, const Vector3& up);
    void MoveForward(double distance);
    void MoveRight(double distance);
    void MoveUp(double distance);
    void RotateHorizontal(double angle);
    void RotateVertical(double angle);
    void RotateRoll(double angle);

    const Near near_;
    const Matrix4 projection_matrix_;
    Vector3 position_;
    Vector3 front_;
    Vector3 up_;
    Vector3 right_;
    Matrix4 view_matrix_;
    Matrix4 view_projection_matrix_;
};
}  // namespace renderer
