#include "object.h"

Object::Object(std::initializer_list<Triangle> list, const Eigen::Matrix3d &rotation,
               const Eigen::Vector3d &translation)
    : object_(list), rotation_(rotation), translation_(translation) {
}

void Object::AddTriangle(const Triangle &t) {
    object_.push_back(t);
}
