#include "camera.h"
#include <cmath>
#include <cassert>

namespace renderer {
Camera::Camera(Width width, Height height, Fov fov, Near near, Far far)
    : aspect_(static_cast<double>(width) / height), fov_(fov), near_(near), far_(far) {

    assert(fov_.value > 0 && fov.value < M_PI);
    assert(aspect_.value > 0);
    assert(near_.value > 0);
    assert(near_.value < far.value);

    const double tan_half_fov = std::tan(fov.value / 2.0);
    projection_matrix_ = kZeroMatrix4;
    projection_matrix_(0, 0) = 1.0 / (aspect_.value * tan_half_fov);
    projection_matrix_(1, 1) = 1.0 / tan_half_fov;
    projection_matrix_(2, 2) = -(far_.value + near_.value) / (far_.value - near_.value);
    projection_matrix_(2, 3) = -(2.0 * far_.value * near_.value) / (far_.value - near_.value);
    projection_matrix_(3, 2) = -1.0;

    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
    Vector3 target = position_ + front_;
    Vector3 f = (target - position_).normalized();
    Vector3 r = f.cross(up_).normalized();
    Vector3 u = r.cross(f);

    view_matrix_ = kIdentityMatrix4;
    view_matrix_(0, 0) = r.x();
    view_matrix_(0, 1) = r.y();
    view_matrix_(0, 2) = r.z();
    view_matrix_(1, 0) = u.x();
    view_matrix_(1, 1) = u.y();
    view_matrix_(1, 2) = u.z();
    view_matrix_(2, 0) = -f.x();
    view_matrix_(2, 1) = -f.y();
    view_matrix_(2, 2) = -f.z();
    view_matrix_(0, 3) = -r.dot(position_);
    view_matrix_(1, 3) = -u.dot(position_);
    view_matrix_(2, 3) = f.dot(position_);
}

void Camera::UpdateViewProjectionMatrix() {
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

const Matrix4& Camera::GetViewProjectionMatrix() const {
    return view_projection_matrix_;
}

void Camera::MoveForward(double distance) {
    position_ += front_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::MoveBackward(double distance) {
    position_ -= front_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::MoveRight(double distance) {
    position_ += right_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::MoveLeft(double distance) {
    position_ -= right_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::MoveUp(double distance) {
    position_ += up_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::MoveDown(double distance) {
    position_ -= up_ * distance;
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::RotateHorizontal(double angle) {
    front_ = (front_ * std::cos(angle) + right_ * std::sin(angle)).normalized();
    right_ = front_.cross(up_).normalized();
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::RotateVertical(double angle) {
    front_ = (front_ * std::cos(angle) + up_ * std::sin(angle)).normalized();
    up_ = right_.cross(front_).normalized();
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::RotateRoll(double angle) {
    up_ = (up_ * std::cos(angle) - right_ * std::sin(angle)).normalized();
    right_ = front_.cross(up_).normalized();
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}
}  // namespace renderer
