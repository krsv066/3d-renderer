#include "object.h"

namespace renderer {
Object::Object(std::vector<Triangle> &&triangles, const Vector3 &translation,
               const Matrix3 &rotation, Color color)
    : object_(std::move(triangles)), translation_(translation), rotation_(rotation), color_(color) {
}

const std::vector<Triangle> &Object::GetTriangles() const {
    return object_;
}

const Vector3 &Object::GetTranslation() const {
    return translation_;
}

const Matrix3 &Object::GetRotation() const {
    return rotation_;
}

Color Object::GetColor() const {
    return color_;
}
}  // namespace renderer
