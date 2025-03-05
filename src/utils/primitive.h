#pragma once

#include "linalg.h"
#include <cstdint>

namespace renderer::primitive {
struct Triangle {
    Vector3 a;
    Vector3 b;
    Vector3 c;
    uint32_t color;
};
}  // namespace renderer::primitive
