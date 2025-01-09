#pragma once

#include "geometry.h"

#include <vector>

class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<Triangle> list);

    template <typename... Args>
    void AddTriangle(Triangle t, Args... args) {
        obj_.push_back(t);
        if constexpr (sizeof...(args) > 0) {
            AddTriangle(args...);
        }
    }

    void AddTriangle(Triangle t);

private:
    std::vector<Triangle> obj_;
};
