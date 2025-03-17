#pragma once

#include "screen.h"
#include <algorithm>
#include <limits>
#include <cassert>

namespace renderer {
Screen::Screen(Width width, Height height)
    : width_(width),
      height_(height),
      z_buffer_(width_ * height_, std::numeric_limits<double>::infinity()),
      frame_buffer_(width_ * height_ * 4, 0) {
    assert(width_ > 0);
    assert(height_ > 0);
}

Screen::Screen(Screen&& other) noexcept
    : width_(other.width_),
      height_(other.height_),
      z_buffer_(std::move(other.z_buffer_)),
      frame_buffer_(std::move(other.frame_buffer_)) {
    other.width_ = Width{0};
    other.height_ = Height{0};
}

Screen& Screen::operator=(Screen&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        z_buffer_ = std::move(other.z_buffer_);
        frame_buffer_ = std::move(other.frame_buffer_);
        other.width_ = Width{0};
        other.height_ = Height{0};
    }
    return *this;
}

double Screen::GetZBufferDepth(int x, int y) const {
    return z_buffer_[GetZBufIdx(x, y)];
}

const uint8_t* Screen::GetFrameBuffer() const {
    assert(!frame_buffer_.empty());
    return frame_buffer_.data();
}

Width Screen::GetWidth() const {
    return width_;
}

Height Screen::GetHeight() const {
    return height_;
}

void Screen::SetZBufferDepth(int x, int y, double z) {
    z_buffer_[GetZBufIdx(x, y)] = z;
}

void Screen::SetFrameBufferPixel(int x, int y, uint32_t color) {
    int index = GetFBufIdx(x, y);
    frame_buffer_[index] = (color >> 16) & 0xFF;
    frame_buffer_[index + 1] = (color >> 8) & 0xFF;
    frame_buffer_[index + 2] = color & 0xFF;
    frame_buffer_[index + 3] = 0xFF;
}

void Screen::Clear() {
    std::fill(frame_buffer_.begin(), frame_buffer_.end(), 0);
    std::fill(z_buffer_.begin(), z_buffer_.end(), std::numeric_limits<double>::infinity());
}

inline int Screen::GetZBufIdx(int x, int y) const {
    assert(x >= 0 && x < width_);
    assert(y >= 0 && y < height_);
    return (height_ - y - 1) * width_ + x;
}

inline int Screen::GetFBufIdx(int x, int y) const {
    return GetZBufIdx(x, y) * 4;
}
}  // namespace renderer
