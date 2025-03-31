#pragma once

#include <cstdint>

namespace renderer {
class Color {
public:
    Color(uint32_t hex);
    uint8_t Red() const;
    uint8_t Green() const;
    uint8_t Blue() const;
    Color operator+(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator*(double scalar) const;
    Color& operator+=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator*=(double scalar);
    static Color Black();
    static Color White();
    static Color DefaultAmbient();
    static Color DefaultDirectional();

private:
    Color(double r, double g, double b);

    static constexpr double kMaxHexColorComponent = 255.0;
    static constexpr int kRedShift = 16;
    static constexpr int kGreenShift = 8;
    static constexpr uint32_t kComponentMask = 0xff;
    double r_;
    double g_;
    double b_;
};

Color operator*(double scalar, const Color& color);
}  // namespace renderer
