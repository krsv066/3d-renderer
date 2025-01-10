#pragma once

#include "geometry.h"

#include <vector>

class Object {
    friend class Renderer;

public:
    Object(std::initializer_list<Triangle> list);

    void AddTriangle(Triangle t);

    template <typename... Args>
    void AddTriangle(Triangle t, Args... args) {
        obj_.push_back(t);
        if constexpr (sizeof...(args) > 0) {
            AddTriangle(args...);
        }
    }

private:
    std::vector<Triangle> obj_;
};
