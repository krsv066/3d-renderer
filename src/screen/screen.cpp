#pragma once

#include "screen.h"

Screen::Screen(Width width, Height height) : width_(width), height_(height) {
}

Width Screen::GetWidth() const {
    return width_;
}

Height Screen::GetHeight() const {
    return height_;
}
