#pragma once

#include "object.h"
#include <vector>

class World {
public:
    World(std::initializer_list<Object> list);

    void AddObject(const Object& obj);

    template <typename... Args>
    void AddObject(const Object& obj, Args... args) {
        objects_.push_back(obj);
        if constexpr (sizeof...(args) > 0) {
            AddObject(args...);
        }
    }

private:
    std::vector<Object> objects_;
};
