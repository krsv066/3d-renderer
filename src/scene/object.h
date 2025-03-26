#pragma once

#include "color.h"
#include "triangle.h"
#include <vector>

namespace renderer {
class Object {
public:
    Object(std::vector<Triangle> &&triangles, const Vector3 &translation = kZeroVector3,
           const Matrix3 &rotation = kIdentityMatrix3, Color color = kWhiteColor);
    const std::vector<Triangle> &GetTriangles() const;
    const Vector3 &GetTranslation() const;
    const Matrix3 &GetRotation() const;
    Color GetColor() const;

private:
    std::vector<Triangle> object_;
    Vector3 translation_;
    Matrix3 rotation_;
    Color color_;
};
}  // namespace renderer
