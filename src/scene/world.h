#pragma once

#include "object.h"
#include "light.h"

namespace renderer {
class World {
public:
    World(std::vector<Object> list);
    const std::vector<Object> &GetObjects() const;
    const std::vector<Light> &GetLights() const;
    void AddLight(const Light &light);

    template <typename... Objects>
    void Add(Objects &&...objects) {
        (objects_.push_back(std::forward<Objects>(objects)), ...);
    }

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
};
}  // namespace renderer
