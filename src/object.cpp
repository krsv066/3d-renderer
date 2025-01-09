#include "object.h"

Object::Object(std::initializer_list<Triangle> list) : obj_(list) {
}

void Object::AddTriangle(Triangle t) {
    obj_.push_back(t);
}
