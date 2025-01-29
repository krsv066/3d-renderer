#pragma once

#include "screen.h"

Screen::Screen(uint32_t width, uint32_t height) : width_(width), height_(height) {
}

uint32_t Screen::GetWidth() const {
    return width_;
}

uint32_t Screen::GetHeight() const {
    return height_;
}
