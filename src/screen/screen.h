#pragma once

#include <cstdint>
#include <vector>

namespace renderer {
enum Width : int;
enum Height : int;

class Screen {
public:
    Screen(Width width = Width{0}, Height height = Height{0});
    Screen(Screen&& other) noexcept;
    Screen& operator=(Screen&& other) noexcept;
    double GetZBufferDepth(int x, int y) const;
    const uint8_t* GetFrameBuffer() const;
    Width GetWidth() const;
    Height GetHeight() const;
    void SetZBufferDepth(int x, int y, double z);
    void SetFrameBufferPixel(int x, int y, uint32_t color);
    void FillBlackColor();

private:
    Width width_;
    Height height_;
    std::vector<double> z_buffer_;
    std::vector<uint8_t> frame_buffer_;

    inline int GetZBufIdx(int x, int y) const;
    inline int GetFBufIdx(int x, int y) const;
};
}  // namespace renderer
