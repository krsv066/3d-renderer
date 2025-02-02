#pragma once

#include "screen.h"

Screen::Screen(Width width, Height height) : width_(width), height_(height) {
}

uint32_t Screen::GetWidth() const {
    return width_;
}

uint32_t Screen::GetHeight() const {
    return height_;
}
