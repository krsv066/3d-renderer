#pragma once

#include "linalg.h"

namespace renderer {
struct Light {
    enum class Type : uint8_t { Ambient, Directional };
    Type type;
    linalg::Vector3 position;
    linalg::Vector3 direction;
    linalg::Vector3 color;
    double intensity;

    static Light Ambient(const linalg::Vector3& color, double intensity) {
        return Light{Type::Ambient, linalg::kZeroVector3, linalg::kZeroVector3, color, intensity};
    }

    static Light Directional(const linalg::Vector3& direction, const linalg::Vector3& color,
                             double intensity) {
        return Light{Type::Directional, linalg::kZeroVector3, direction.normalized(), color,
                     intensity};
    }
};
}  // namespace renderer
