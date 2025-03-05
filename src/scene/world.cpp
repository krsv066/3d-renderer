#include "world.h"

namespace renderer {
World::World(std::initializer_list<Object> list) : objects_(list) {
}

const std::vector<Object> &World::GetObjects() const {
    return objects_;
}
}  // namespace renderer
