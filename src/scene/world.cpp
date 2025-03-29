#include "world.h"

namespace renderer {
World::World(std::vector<Object> objects) : objects_(objects) {
    lights_.push_back(Light::Ambient(kDefaultAmbient, 1.0));
    lights_.push_back(
        Light::Directional(Vector3(-1.0, -1.0, -1.0), Color{Red{1.0}, Green{0.9}, Blue{0.7}}, 0.8));
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
