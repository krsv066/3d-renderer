#pragma once

#include "color.h"
#include "linalg.h"

namespace renderer {
struct Light {
    enum class Type : uint8_t { Ambient, Directional };
    Type type;
    linalg::Vector3 position;
    linalg::Vector3 direction;
    Color color;
    double intensity;

    static Light Ambient(const Color& color, double intensity) {
        return Light{Type::Ambient, linalg::kZeroVector3, linalg::kZeroVector3, color, intensity};
    }

    static Light Directional(const linalg::Vector3& direction, const Color& color,
                             double intensity) {
        return Light{Type::Directional, linalg::kZeroVector3, direction.normalized(), color,
                     intensity};
    }
};
}  // namespace renderer
