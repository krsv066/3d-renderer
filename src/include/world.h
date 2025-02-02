#pragma once

#include "object.h"
#include <vector>

class World {
    friend class Renderer;

public:
    World(std::initializer_list<Object> list);

    template <typename... Objects>
    void Add(Objects&&... objects) {
        (objects_.push_back(std::forward<Objects>(objects)), ...);
    }

private:
    std::vector<Object> objects_;
};
