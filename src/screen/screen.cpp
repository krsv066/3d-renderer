#pragma once

#include "screen.h"

Screen::Screen(Width width, Height height)
    : width_(width),
      height_(height),
      z_buffer_(width_ * height_, std::numeric_limits<double>::infinity()),
      frame_buffer_(width_ * height_ * 4, 0) {
}

void Screen::SetFrameBufferPixel(int index, uint32_t color) {
    frame_buffer_[index] = (color >> 16) & 0xFF;
    frame_buffer_[index + 1] = (color >> 8) & 0xFF;
    frame_buffer_[index + 2] = color & 0xFF;
    frame_buffer_[index + 3] = 0xFF;
}

void Screen::SetZBufferDepth(int index, double z) {
    z_buffer_[index] = z;
}

const uint8_t* Screen::GetFrameBuffer() const {
    return frame_buffer_.data();
}

double Screen::GetZBufferDepth(int index) const {
    return z_buffer_[index];
}

Width Screen::GetWidth() const {
    return width_;
}

Height Screen::GetHeight() const {
    return height_;
}
