#pragma once

#include "color.h"
#include "triangle.h"
#include <vector>

namespace renderer {
class Object {
public:
    Object(std::vector<primitive::Triangle> &&triangles,
           const linalg::Vector3 &translation = linalg::kZeroVector3,
           const linalg::Matrix3 &rotation = linalg::kIdentityMatrix3, Color color = kWhiteColor);
    const std::vector<primitive::Triangle> &GetTriangles() const;
    const linalg::Vector3 &GetTranslation() const;
    const linalg::Matrix3 &GetRotation() const;
    Color GetColor() const;

private:
    std::vector<primitive::Triangle> object_;
    linalg::Vector3 translation_;
    linalg::Matrix3 rotation_;
    Color color_;
};
}  // namespace renderer
