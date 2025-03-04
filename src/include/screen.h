#pragma once

#include <cstdint>

enum Width : uint32_t;
enum Height : uint32_t;

class Screen {
public:
    Screen(Width width, Height height);

    Width GetWidth() const;

    Height GetHeight() const;

private:
    Width width_;
    Height height_;
};
