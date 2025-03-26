#pragma once

#include <Eigen/Dense>

namespace renderer {
using AngleAxis = Eigen::AngleAxisd;
using Vector3 = Eigen::Vector3d;
using Vector4 = Eigen::Vector4d;
using Matrix3 = Eigen::Matrix3d;
using Matrix4 = Eigen::Matrix4d;

const auto kZeroVector3 = Eigen::Vector3d::Zero();
const auto kZeroVector4 = Eigen::Vector4d::Zero();
const auto kZeroMatrix3 = Eigen::Matrix3d::Zero();
const auto kZeroMatrix4 = Eigen::Matrix4d::Zero();
const auto kIdentityMatrix3 = Eigen::Matrix3d::Identity();
const auto kIdentityMatrix4 = Eigen::Matrix4d::Identity();
}  // namespace renderer
