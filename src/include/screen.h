#pragma once

#include <cstdint>
#include <vector>

enum Width : uint32_t;
enum Height : uint32_t;

class Screen {
public:
    Screen(Width width, Height height);

    void SetFrameBufferPixel(int index, uint32_t color);

    void SetZBufferDepth(int index, double z);

    const uint8_t* GetFrameBuffer() const;

    double GetZBufferDepth(int index) const;

    Width GetWidth() const;

    Height GetHeight() const;

private:
    Width width_;
    Height height_;
    std::vector<double> z_buffer_;
    std::vector<uint8_t> frame_buffer_;
};
