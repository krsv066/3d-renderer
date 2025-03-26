#pragma once

#include "color.h"
#include "linalg.h"

namespace renderer {
struct Triangle {
    Vector3 a;
    Vector3 b;
    Vector3 c;
    Vector3 normal;
    Color color;
};

struct TriangleProjected {
    Vector4 a;
    Vector4 b;
    Vector4 c;
    Vector3 normal;
    Color color;
};
}  // namespace renderer
