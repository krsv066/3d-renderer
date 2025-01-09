#pragma once

#include <cstdint>

struct Point {
    double x, y, z;
};

struct Triangle {
    Point a, b, c;
    uint32_t color;
};
