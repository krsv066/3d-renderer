#include "object.h"

Object::Object(std::initializer_list<Triangle> list) : object_(list) {
}

void Object::AddTriangle(const Triangle &t) {
    object_.push_back(t);
}
