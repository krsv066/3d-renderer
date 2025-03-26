#pragma once

#include "color.h"
#include "linalg.h"

namespace renderer {
struct Light {
    enum class Type : uint8_t { Ambient, Directional };
    Type type;
    Vector3 position;
    Vector3 direction;
    Color color;
    double intensity;

    static Light Ambient(const Color& color, double intensity) {
        return Light{Type::Ambient, kZeroVector3, kZeroVector3, color, intensity};
    }

    static Light Directional(const Vector3& direction, const Color& color, double intensity) {
        return Light{Type::Directional, kZeroVector3, direction.normalized(), color, intensity};
    }
};
}  // namespace renderer
