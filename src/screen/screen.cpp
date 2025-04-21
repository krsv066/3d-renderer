#include "screen.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>

namespace renderer {
Screen::Screen(Width width, Height height) : width_(width), z_buffer_(width * height, std::numeric_limits<double>::infinity()), frame_buffer_(width * height * 4, 0) {
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
    return static_cast<Height>(z_buffer_.size() / static_cast<size_t>(width_));
}

void Screen::SetZBufferDepth(int x, int y, double z) {
    z_buffer_[GetZBufIdx(x, y)] = z;
}

void Screen::SetFrameBufferPixel(int x, int y, Color color) {
    int index = GetFBufIdx(x, y);
    frame_buffer_[index] = color.Red();
    frame_buffer_[index + 1] = color.Green();
    frame_buffer_[index + 2] = color.Blue();
    frame_buffer_[index + 3] = 0xff;
}

void Screen::ResetBuffers() {
    std::fill(frame_buffer_.begin(), frame_buffer_.end(), 0);
    std::fill(z_buffer_.begin(), z_buffer_.end(), std::numeric_limits<double>::infinity());
}

inline int Screen::GetZBufIdx(int x, int y) const {
    assert(x >= 0 && x < width_);
    assert(y >= 0 && y < height_);
    return (GetHeight() - y - 1) * width_ + x;
}

inline int Screen::GetFBufIdx(int x, int y) const {
    return GetZBufIdx(x, y) * 4;
}
}  // namespace renderer
