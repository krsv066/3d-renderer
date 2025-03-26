#pragma once

#include "object.h"
#include "light.h"

namespace renderer {
class World {
public:
    World(std::vector<Object> objects);
    const std::vector<Object> &GetObjects() const;
    const std::vector<Light> &GetLights() const;
    void AddLight(const Light &light);

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
};
}  // namespace renderer
