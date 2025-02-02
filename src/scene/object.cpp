#include "object.h"

Object::Object(std::initializer_list<Triangle> list, const Eigen::Vector3d &translation,
               const Eigen::Matrix3d &rotation)
    : object_(list), translation_(translation), rotation_(rotation) {
}

Object::Object(const std::vector<Triangle> &triangles, const Eigen::Vector3d &translation,
               const Eigen::Matrix3d &rotation)
    : object_(triangles), translation_(translation), rotation_(rotation) {
}
