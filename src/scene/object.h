#pragma once

#include "primitive.h"
#include <cstdint>
#include <vector>

namespace renderer {
class Object {
public:
    Object(std::initializer_list<primitive::Triangle> list,
           const linalg::Vector3 &translation = linalg::kZeroVector3,
           const linalg::Matrix3 &rotation = linalg::kIdentityMatrix3, uint32_t color = 0xFFFFFF);
    Object(const std::vector<primitive::Triangle> &triangles,
           const linalg::Vector3 &translation = linalg::kZeroVector3,
           const linalg::Matrix3 &rotation = linalg::kIdentityMatrix3, uint32_t color = 0xFFFFFF);
    const std::vector<primitive::Triangle> &GetTriangles() const;
    const linalg::Vector3 &GetTranslation() const;
    const linalg::Matrix3 &GetRotation() const;
    uint32_t GetColor() const;
    template <typename... Triangles>
    void Add(Triangles &&...triangles) {
        (object_.push_back(std::forward<Triangles>(triangles)), ...);
    }

private:
    std::vector<primitive::Triangle> object_;
    linalg::Vector3 translation_;
    linalg::Matrix3 rotation_;
    uint32_t color_;
};
}  // namespace renderer
