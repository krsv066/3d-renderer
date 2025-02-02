#pragma once

#include <cstdint>

enum Width : uint32_t;
enum Height : uint32_t;

class Screen {
public:
    Screen(Width width, Height height);

    uint32_t GetWidth() const;

    uint32_t GetHeight() const;

private:
    Width width_;
    uint32_t height_;
};
