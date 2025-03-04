#include "object.h"

namespace renderer {
Object::Object(std::initializer_list<primitive::Triangle> list, const Eigen::Vector3d &translation,
               const Eigen::Matrix3d &rotation)
    : object_(list), translation_(translation), rotation_(rotation) {
}

Object::Object(const std::vector<primitive::Triangle> &triangles,
               const Eigen::Vector3d &translation, const Eigen::Matrix3d &rotation)
    : object_(triangles), translation_(translation), rotation_(rotation) {
}
}  // namespace renderer
