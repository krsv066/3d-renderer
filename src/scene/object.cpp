#include "object.h"

namespace renderer {
Object::Object(std::initializer_list<primitive::Triangle> list, const linalg::Vector3 &translation,
               const linalg::Matrix3 &rotation)
    : object_(list), translation_(translation), rotation_(rotation) {
}

Object::Object(const std::vector<primitive::Triangle> &triangles,
               const linalg::Vector3 &translation, const linalg::Matrix3 &rotation)
    : object_(triangles), translation_(translation), rotation_(rotation) {
}

const std::vector<primitive::Triangle> &Object::GetTriangles() const {
    return object_;
}

const linalg::Vector3 &Object::GetTranslation() const {
    return translation_;
}

const linalg::Matrix3 &Object::GetRotation() const {
    return rotation_;
}
}  // namespace renderer
