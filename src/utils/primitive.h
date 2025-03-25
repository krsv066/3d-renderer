#pragma once

#include "linalg.h"

namespace renderer::primitive {
struct Triangle {
    linalg::Vector3 a;
    linalg::Vector3 b;
    linalg::Vector3 c;
};
}  // namespace renderer::primitive
