#pragma once

#include "primitive.h"
#include <vector>

class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<Triangle> list,
           const Eigen::Vector3d &translation = Eigen::Vector3d::Zero(),
           const Eigen::Matrix3d &rotation = Eigen::Matrix3d::Identity());

    Object(const std::vector<Triangle> &triangles,
           const Eigen::Vector3d &translation = Eigen::Vector3d::Zero(),
           const Eigen::Matrix3d &rotation = Eigen::Matrix3d::Identity());

    void AddTriangle(const Triangle &t);

    template <typename... Args>
    void AddTriangle(const Triangle &t, Args... args) {
        object_.push_back(t);
        if constexpr (sizeof...(args) > 0) {
            AddTriangle(args...);
        }
    }

private:
    std::vector<Triangle> object_;
    Eigen::Vector3d translation_;
    Eigen::Matrix3d rotation_;
};
