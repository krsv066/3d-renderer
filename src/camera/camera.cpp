#include "camera.h"
#include <cmath>
#include <cassert>

namespace renderer {
Aspect::Aspect(double value) : value_(value) {
}

double Aspect::Get() const {
    return value_;
}

Fov::Fov(double value) : value_(value * M_PI / 180.0) {
}

double Fov::Get() const {
    return value_;
}

Near::Near(double value) : value_(value) {
}

double Near::Get() const {
    return value_;
}

Far::Far(double value) : value_(value) {
}

double Far::Get() const {
    return value_;
}

Camera::Camera(Width width, Height height, Fov fov, Near near, Far far)
    : near_(near),
      projection_matrix_(MakeProjectionMatrix(width, height, fov, near, far)),
      position_(Vector3(0, 0, 0)),
      front_(Vector3(0, 0, -1)),
      up_(Vector3(0, 1, 0)),
      right_(Vector3(1, 0, 0)),
      view_matrix_(MakeViewMatrix(position_, front_, up_)),
      view_projection_matrix_(projection_matrix_ * view_matrix_) {
    assert(fov_.value > 0.0 && fov.value < M_PI);
    assert(aspect_.value > 0.0);
    assert(near_.value > 0.0);
    assert(near_.value < far.value);
}

const Matrix4& Camera::GetViewProjectionMatrix() const {
    return view_projection_matrix_;
}

void Camera::Move(const Movement& movement) {
    if (movement.forward_movement != 0.0) {
        MoveForward(movement.forward_movement);
    }
    if (movement.right_movement != 0.0) {
        MoveRight(movement.right_movement);
    }
    if (movement.up_movement != 0.0) {
        MoveUp(movement.up_movement);
    }
    if (movement.horizontal_rotation != 0.0) {
        RotateHorizontal(movement.horizontal_rotation);
    }
    if (movement.vertical_rotation != 0.0) {
        RotateVertical(movement.vertical_rotation);
    }
    if (movement.roll_rotation != 0.0) {
        RotateRoll(movement.roll_rotation);
    }

    view_matrix_ = MakeViewMatrix(position_, front_, up_);
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

double Camera::GetNear() const {
    return near_.Get();
}

Matrix4 Camera::MakeProjectionMatrix(Width width, Height height, Fov fov, Near near, Far far) {
    const double tan_half_fov = std::tan(fov.Get() / 2.0);
    const double aspect = static_cast<double>(width) / height;

    Matrix4 projection_matrix = Matrix4::Zero();
    projection_matrix(0, 0) = 1.0 / (aspect * tan_half_fov);
    projection_matrix(1, 1) = 1.0 / tan_half_fov;
    projection_matrix(2, 2) = -(far.Get() + near.Get()) / (far.Get() - near.Get());
    projection_matrix(2, 3) = -(2.0 * far.Get() * near.Get()) / (far.Get() - near.Get());
    projection_matrix(3, 2) = -1.0;

    return projection_matrix;
}

Matrix4 Camera::MakeViewMatrix(const Vector3& position, const Vector3& front, const Vector3& up) {
    const Vector3 f = front.normalized();
    const Vector3 r = f.cross(up).normalized();
    const Vector3 u = r.cross(f);

    Matrix4 view_matrix = Matrix4::Identity();
    view_matrix(0, 0) = r.x();
    view_matrix(0, 1) = r.y();
    view_matrix(0, 2) = r.z();
    view_matrix(1, 0) = u.x();
    view_matrix(1, 1) = u.y();
    view_matrix(1, 2) = u.z();
    view_matrix(2, 0) = -f.x();
    view_matrix(2, 1) = -f.y();
    view_matrix(2, 2) = -f.z();
    view_matrix(0, 3) = -r.dot(position);
    view_matrix(1, 3) = -u.dot(position);
    view_matrix(2, 3) = f.dot(position);

    return view_matrix;
}

void Camera::MoveForward(double distance) {
    position_ += front_ * distance;
}

void Camera::MoveRight(double distance) {
    position_ += right_ * distance;
}

void Camera::MoveUp(double distance) {
    position_ += up_ * distance;
}

void Camera::RotateHorizontal(double angle) {
    front_ = (front_ * std::cos(angle) + right_ * std::sin(angle)).normalized();
    right_ = front_.cross(up_).normalized();
}

void Camera::RotateVertical(double angle) {
    front_ = (front_ * std::cos(angle) + up_ * std::sin(angle)).normalized();
    up_ = right_.cross(front_).normalized();
}

void Camera::RotateRoll(double angle) {
    up_ = (up_ * std::cos(angle) - right_ * std::sin(angle)).normalized();
    right_ = front_.cross(up_).normalized();
}
}  // namespace renderer
