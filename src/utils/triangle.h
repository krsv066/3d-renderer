#pragma once

#include "color.h"
#include "linalg.h"

namespace renderer {
struct Triangle {
    Matrix3 vertices;
    Vector3 normal;
    Color color;
};

struct TriangleProjected {
    Matrix43 vertices;
    Vector3 normal;
    Color color;
};
}  // namespace renderer
