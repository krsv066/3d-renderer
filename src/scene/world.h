#pragma once

#include "object.h"

namespace renderer {
class World {
public:
    World(std::initializer_list<Object> list);
    const std::vector<Object> &GetObjects() const;
    template <typename... Objects>
    void Add(Objects &&...objects) {
        (objects_.push_back(std::forward<Objects>(objects)), ...);
    }

private:
    std::vector<Object> objects_;
};
}  // namespace renderer
