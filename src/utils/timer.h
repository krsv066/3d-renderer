#pragma once

#include <chrono>

namespace renderer {
class Timer {
public:
    Timer();

    double Tick();

private:
    std::chrono::high_resolution_clock::time_point last_frame_time_;
    double delta_time_;
};
}  // namespace renderer
