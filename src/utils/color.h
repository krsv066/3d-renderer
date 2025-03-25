#pragma once

#include <algorithm>
#include <cstdint>

namespace renderer {
struct Red {
    double value;
    explicit constexpr Red(double value) : value(std::clamp(value, 0.0, 1.0)) {
    }
};

struct Green {
    double value;
    explicit constexpr Green(double value) : value(std::clamp(value, 0.0, 1.0)) {
    }
};

struct Blue {
    double value;
    explicit constexpr Blue(double value) : value(std::clamp(value, 0.0, 1.0)) {
    }
};

class Color {
public:
    explicit constexpr Color(Red r = Red{0.0}, Green g = Green{0.0}, Blue b = Blue{0.0})
        : r_(r), g_(g), b_(b) {
    }

    uint32_t GetHex() const;
    Color operator+(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator*(double scalar) const;
    Color& operator+=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator*=(double scalar);

private:
    Red r_;
    Green g_;
    Blue b_;
};

Color operator*(double scalar, const Color& color);

static constexpr Color kBlackColor = Color{Red{0.0}, Green{0.0}, Blue{0.0}};
static constexpr Color kWhiteColor = Color{Red{1.0}, Green{1.0}, Blue{1.0}};
static constexpr Color kDefaultAmbient = Color{Red{0.1}, Green{0.1}, Blue{0.1}};
}  // namespace renderer
