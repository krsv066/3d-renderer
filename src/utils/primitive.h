#pragma once

#include "linalg.h"
#include <cstdint>

namespace renderer::primitive {
struct Triangle {
    linalg::Vector3 a;
    linalg::Vector3 b;
    linalg::Vector3 c;
    uint32_t color;
};
}  // namespace renderer::primitive
