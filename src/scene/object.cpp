#include "object.h"

namespace renderer {
Object::Object(std::initializer_list<primitive::Triangle> list, const Vector3 &translation,
               const Matrix3 &rotation)
    : object_(list), translation_(translation), rotation_(rotation) {
}

Object::Object(const std::vector<primitive::Triangle> &triangles, const Vector3 &translation,
               const Matrix3 &rotation)
    : object_(triangles), translation_(translation), rotation_(rotation) {
}
}  // namespace renderer
