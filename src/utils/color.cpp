#include "color.h"
#include <algorithm>

namespace renderer {
static constexpr double kMaxHexColorComponent = 255.0;

Color::Color(uint32_t hex)
    : r_(Red(((hex >> 16) & 0xFF) / kMaxHexColorComponent)),
      g_(Green(((hex >> 8) & 0xFF) / kMaxHexColorComponent)),
      b_(Blue((hex & 0xFF) / kMaxHexColorComponent)) {
}

uint32_t Color::GetHex() const {
    uint8_t r_component = static_cast<uint8_t>(r_.value * kMaxHexColorComponent);
    uint8_t g_component = static_cast<uint8_t>(g_.value * kMaxHexColorComponent);
    uint8_t b_component = static_cast<uint8_t>(b_.value * kMaxHexColorComponent);

    return (r_component << 16) | (g_component << 8) | b_component;
}

Color Color::operator+(const Color& other) const {
    return Color(Red(std::clamp(r_.value + other.r_.value, 0.0, 1.0)),
                 Green(std::clamp(g_.value + other.g_.value, 0.0, 1.0)),
                 Blue(std::clamp(b_.value + other.b_.value, 0.0, 1.0)));
}

Color Color::operator*(const Color& other) const {
    return Color(Red(std::clamp(r_.value * other.r_.value, 0.0, 1.0)),
                 Green(std::clamp(g_.value * other.g_.value, 0.0, 1.0)),
                 Blue(std::clamp(b_.value * other.b_.value, 0.0, 1.0)));
}

Color Color::operator*(double scalar) const {
    return Color(Red(std::clamp(r_.value * scalar, 0.0, 1.0)),
                 Green(std::clamp(g_.value * scalar, 0.0, 1.0)),
                 Blue(std::clamp(b_.value * scalar, 0.0, 1.0)));
}

Color& Color::operator+=(const Color& other) {
    r_.value = std::clamp(r_.value + other.r_.value, 0.0, 1.0);
    g_.value = std::clamp(g_.value + other.g_.value, 0.0, 1.0);
    b_.value = std::clamp(b_.value + other.b_.value, 0.0, 1.0);
    return *this;
}

Color& Color::operator*=(const Color& other) {
    r_.value = std::clamp(r_.value * other.r_.value, 0.0, 1.0);
    g_.value = std::clamp(g_.value * other.g_.value, 0.0, 1.0);
    b_.value = std::clamp(b_.value * other.b_.value, 0.0, 1.0);
    return *this;
}

Color& Color::operator*=(double scalar) {
    r_.value = std::clamp(r_.value * scalar, 0.0, 1.0);
    g_.value = std::clamp(g_.value * scalar, 0.0, 1.0);
    b_.value = std::clamp(b_.value * scalar, 0.0, 1.0);
    return *this;
}

Color operator*(double scalar, const Color& color) {
    return color * scalar;
}
}  // namespace renderer
