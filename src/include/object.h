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

    template <typename... Triangles>
    void Add(Triangles &&...triangles) {
        (object_.push_back(std::forward<Triangles>(triangles)), ...);
    }

private:
    std::vector<Triangle> object_;
    Eigen::Vector3d translation_;
    Eigen::Matrix3d rotation_;
};
