#pragma once

#include "primitive.h"
#include <vector>

class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<Triangle> list);

    void AddTriangle(const Triangle &t);

    template <typename... Args>
    void AddTriangle(const Triangle &t, Args... args) {
        object_.push_back(t);
        if constexpr (sizeof...(args) > 0) {
            AddTriangle(args...);
        }
    }

private:
    std::vector<Triangle> object_;  // TODO: Ax + v
};
