#include "object.h"

namespace renderer {
Object::Object(std::vector<primitive::Triangle> &&triangles, const linalg::Vector3 &translation,
               const linalg::Matrix3 &rotation, Color color)
    : object_(std::move(triangles)), translation_(translation), rotation_(rotation), color_(color) {
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

Color Object::GetColor() const {
    return color_;
}
}  // namespace renderer
