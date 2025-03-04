#include "world.h"

namespace renderer {
World::World(std::initializer_list<Object> list) : objects_(list) {
}
}  // namespace renderer
