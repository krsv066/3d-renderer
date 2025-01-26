#include "object.h"

Object::Object(std::initializer_list<Triangle> list, const Eigen::Vector3d &translation,
               const Eigen::Matrix3d &rotation)
    : object_(list), translation_(translation), rotation_(rotation) {
}

void Object::AddTriangle(const Triangle &t) {
    object_.push_back(t);
}
