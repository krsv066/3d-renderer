#pragma once

#include "object.h"
#include "light.h"

namespace renderer {
class World {
public:
    World(std::initializer_list<Object> list);
    const std::vector<Object> &GetObjects() const;
    const std::vector<Light> &GetLights() const;

    template <typename... Objects>
    void Add(Objects &&...objects) {
        (objects_.push_back(std::forward<Objects>(objects)), ...);
    }

    void AddLight(const Light &light);

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
};
}  // namespace renderer
