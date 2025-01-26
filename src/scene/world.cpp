#include "world.h"

World::World(std::initializer_list<Object> list) : objects_(list) {
}

void World::AddObject(const Object& obj) {
    objects_.push_back(obj);
}
