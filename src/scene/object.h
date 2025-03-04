#pragma once

#include "primitive.h"
#include <vector>

namespace renderer {
class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<primitive::Triangle> list,
           const Eigen::Vector3d &translation = Eigen::Vector3d::Zero(),
           const Eigen::Matrix3d &rotation = Eigen::Matrix3d::Identity());
    Object(const std::vector<primitive::Triangle> &triangles,
           const Eigen::Vector3d &translation = Eigen::Vector3d::Zero(),
           const Eigen::Matrix3d &rotation = Eigen::Matrix3d::Identity());
    template <typename... Triangles>
    void Add(Triangles &&...triangles) {
        (object_.push_back(std::forward<Triangles>(triangles)), ...);
    }

private:
    std::vector<primitive::Triangle> object_;
    Eigen::Vector3d translation_;
    Eigen::Matrix3d rotation_;
};
}  // namespace renderer
