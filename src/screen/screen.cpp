#pragma once

#include "screen.h"
#include <algorithm>
#include <limits>

namespace renderer {
Screen::Screen(Width width, Height height)
    : width_(width),
      height_(height),
      z_buffer_(width_ * height_, std::numeric_limits<double>::infinity()),
      frame_buffer_(width_ * height_ * 4, 0) {
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

void Screen::Clear() {
    std::fill(frame_buffer_.begin(), frame_buffer_.end(), 0);
    std::fill(z_buffer_.begin(), z_buffer_.end(), std::numeric_limits<double>::infinity());
}
}  // namespace renderer
