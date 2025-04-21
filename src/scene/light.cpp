#include "light.h"

namespace renderer {
Light Light::Ambient(const Color& color, double intensity) {
    return Light{Type::Ambient, Vector3::Zero(), Vector3::Zero(), color, intensity};
}

Light Light::Directional(const Vector3& direction, const Color& color, double intensity) {
    return Light{Type::Directional, Vector3::Zero(), direction.normalized(), color, intensity};
}
}  // namespace renderer
