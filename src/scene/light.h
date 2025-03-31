#pragma once

#include "color.h"
#include "linalg.h"

namespace renderer {
struct Light {
    static Light Ambient(const Color& color, double intensity);
    static Light Directional(const Vector3& direction, const Color& color, double intensity);

    enum class Type : uint8_t { Ambient, Directional };
    Type type;
    Vector3 position;
    Vector3 direction;
    Color color;
    double intensity;
};
}  // namespace renderer
