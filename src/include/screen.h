#pragma once

#include <cstdint>

class Screen {
public:
    Screen(uint32_t width, uint32_t height);

private:
    uint32_t width_;
    uint32_t height_;
};
