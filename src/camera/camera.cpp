#include "camera.h"
#include <cmath>
#include <cassert>

namespace renderer {
Camera::Camera(Fov fov, Aspect aspect, Near near, Far far)
    : fov_(fov), aspect_(aspect), near_(near), far_(far) {
    
    assert(fov.value > 0 && fov.value < M_PI);
    assert(aspect.value > 0);
    assert(near.value > 0);
    assert(near.value < far.value);

    const double tan_half_fov = std::tan(fov.value / 2.0);
    projection_matrix_ = linalg::kZeroMatrix4;
    projection_matrix_(0, 0) = 1.0 / (aspect.value * tan_half_fov);
    projection_matrix_(1, 1) = 1.0 / tan_half_fov;
    projection_matrix_(2, 2) = -(far.value + near.value) / (far.value - near.value);
    projection_matrix_(2, 3) = -(2.0 * far.value * near.value) / (far.value - near.value);
    projection_matrix_(3, 2) = -1.0;

    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
    linalg::Vector3 target = position_ + front_;
    linalg::Vector3 f = (target - position_).normalized();
    linalg::Vector3 r = f.cross(up_).normalized();
    linalg::Vector3 u = r.cross(f);

    view_matrix_ = linalg::kIdentityMatrix4;
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

const linalg::Matrix4& Camera::GetViewProjectionMatrix() const {
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
}  // namespace renderer
