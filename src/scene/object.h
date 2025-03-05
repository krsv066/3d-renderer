#pragma once

#include "primitive.h"
#include <vector>

namespace renderer {
class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<primitive::Triangle> list,
           const Vector3 &translation = Vector3::Zero(),
           const Matrix3 &rotation = Matrix3::Identity());
    Object(const std::vector<primitive::Triangle> &triangles,
           const Vector3 &translation = Vector3::Zero(),
           const Matrix3 &rotation = Matrix3::Identity());
    template <typename... Triangles>
    void Add(Triangles &&...triangles) {
        (object_.push_back(std::forward<Triangles>(triangles)), ...);
    }

private:
    std::vector<primitive::Triangle> object_;
    Vector3 translation_;
    Matrix3 rotation_;
};
}  // namespace renderer
