#pragma once

#include <cstdint>

namespace renderer {
class Color {
public:
    Color(uint32_t hex);
    uint32_t GetHex() const;
    Color operator+(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator*(double scalar) const;
    Color& operator+=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator*=(double scalar);

private:
    double r_;
    double g_;
    double b_;

    Color(double r, double g, double b);
};

Color operator*(double scalar, const Color& color);

static const Color kBlackColor = Color(0x000000);
static const Color kWhiteColor = Color{0xffffff};
static const Color kDefaultAmbient = Color{0x1a1a1a};
static const Color kDefaultDirectional = Color{0xffe6b2};
}  // namespace renderer
