#include "color.h"
#include <algorithm>

namespace renderer {
Color::Color(uint32_t hex)
    : r_(((hex >> kRedShift) & kComponentMask) / kMaxHexColorComponent), g_(((hex >> kGreenShift) & kComponentMask) / kMaxHexColorComponent), b_((hex & kComponentMask) / kMaxHexColorComponent) {
}

Color::Color(double r, double g, double b) : r_(r), g_(g), b_(b) {
}

uint8_t Color::Red() const {
    return static_cast<uint8_t>(r_ * kMaxHexColorComponent);
}

uint8_t Color::Green() const {
    return static_cast<uint8_t>(g_ * kMaxHexColorComponent);
}

uint8_t Color::Blue() const {
    return static_cast<uint8_t>(b_ * kMaxHexColorComponent);
}

Color Color::operator+(const Color& other) const {
    Color result = *this;
    result += other;
    return result;
}

Color Color::operator*(const Color& other) const {
    Color result = *this;
    result *= other;
    return result;
}

Color Color::operator*(double scalar) const {
    return Color(std::clamp(r_ * scalar, 0.0, 1.0), std::clamp(g_ * scalar, 0.0, 1.0), std::clamp(b_ * scalar, 0.0, 1.0));
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

Color Color::Black() {
    return Color{0x000000};
}

Color Color::White() {
    return Color{0xffffff};
}

Color Color::DefaultAmbient() {
    return Color{0x1a1a1a};
}

Color Color::DefaultDirectional() {
    return Color{0xffe6b2};
}

Color operator*(double scalar, const Color& color) {
    return color * scalar;
}
}  // namespace renderer
