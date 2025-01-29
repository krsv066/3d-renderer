#pragma once

#include <cstdint>

class Screen {
public:
    Screen(uint32_t width, uint32_t height);

    uint32_t GetWidth() const;

    uint32_t GetHeight() const;

private:
    uint32_t width_;
    uint32_t height_;
};
