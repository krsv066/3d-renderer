#include "color.h"
#include <algorithm>

namespace renderer {
static constexpr double kMaxHexColorComponent = 255.0;
static constexpr int kRedShift = 16;
static constexpr int kGreenShift = 8;
static constexpr uint32_t kComponentMask = 0xff;

Color::Color(uint32_t hex)
    : r_(((hex >> kRedShift) & kComponentMask) / kMaxHexColorComponent),
      g_(((hex >> kGreenShift) & kComponentMask) / kMaxHexColorComponent),
      b_((hex & kComponentMask) / kMaxHexColorComponent) {
}

Color::Color(double r, double g, double b) : r_(r), g_(g), b_(b) {
}

uint32_t Color::GetHex() const {
    uint8_t r_component = static_cast<uint8_t>(r_ * kMaxHexColorComponent);
    uint8_t g_component = static_cast<uint8_t>(g_ * kMaxHexColorComponent);
    uint8_t b_component = static_cast<uint8_t>(b_ * kMaxHexColorComponent);

    return (r_component << kRedShift) | (g_component << kGreenShift) | b_component;
}

Color Color::operator+(const Color& other) const {
    return Color(std::clamp(r_ + other.r_, 0.0, 1.0), std::clamp(g_ + other.g_, 0.0, 1.0),
                 std::clamp(b_ + other.b_, 0.0, 1.0));
}

Color Color::operator*(const Color& other) const {
    return Color(std::clamp(r_ * other.r_, 0.0, 1.0), std::clamp(g_ * other.g_, 0.0, 1.0),
                 std::clamp(b_ * other.b_, 0.0, 1.0));
}

Color Color::operator*(double scalar) const {
    return Color(std::clamp(r_ * scalar, 0.0, 1.0), std::clamp(g_ * scalar, 0.0, 1.0),
                 std::clamp(b_ * scalar, 0.0, 1.0));
}

Color& Color::operator+=(const Color& other) {
    r_ = std::clamp(r_ + other.r_, 0.0, 1.0);
    g_ = std::clamp(g_ + other.g_, 0.0, 1.0);
    b_ = std::clamp(b_ + other.b_, 0.0, 1.0);
    return *this;
}

Color& Color::operator*=(const Color& other) {
    r_ = std::clamp(r_ * other.r_, 0.0, 1.0);
    g_ = std::clamp(g_ * other.g_, 0.0, 1.0);
    b_ = std::clamp(b_ * other.b_, 0.0, 1.0);
    return *this;
}

Color& Color::operator*=(double scalar) {
    r_ = std::clamp(r_ * scalar, 0.0, 1.0);
    g_ = std::clamp(g_ * scalar, 0.0, 1.0);
    b_ = std::clamp(b_ * scalar, 0.0, 1.0);
    return *this;
}

Color operator*(double scalar, const Color& color) {
    return color * scalar;
}
}  // namespace renderer
