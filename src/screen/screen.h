#pragma once

#include "color.h"
#include <cstdint>
#include <vector>

namespace renderer {
enum Width : int;
enum Height : int;

class Screen {
public:
    Screen(Width width = Width{0}, Height height = Height{0});
    double GetZBufferDepth(int x, int y) const;
    const uint8_t* GetFrameBuffer() const;
    Width GetWidth() const;
    Height GetHeight() const;
    void SetZBufferDepth(int x, int y, double z);
    void SetFrameBufferPixel(int x, int y, Color color);
    void ResetBuffers();

private:
    int GetZBufIdx(int x, int y) const;
    int GetFBufIdx(int x, int y) const;

    Width width_;
    std::vector<double> z_buffer_;
    std::vector<uint8_t> frame_buffer_;
};
}  // namespace renderer
