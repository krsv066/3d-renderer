#include "world.h"

namespace renderer {
World::World(std::vector<Object> list) : objects_(list) {
    lights_.push_back(Light::Ambient(kDefaultAmbient, 1.0));
}

const std::vector<Object> &World::GetObjects() const {
    return objects_;
}

const std::vector<Light> &World::GetLights() const {
    return lights_;
}

void World::AddLight(const Light &light) {
    lights_.push_back(light);
}
}  // namespace renderer
