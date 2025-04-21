#include "timer.h"

namespace renderer {
Timer::Timer() : last_frame_time_(std::chrono::high_resolution_clock::now()), delta_time_(0.0) {
}

double Timer::Tick() {
    auto current_frame_time = std::chrono::high_resolution_clock::now();
    delta_time_ = std::chrono::duration<double>(current_frame_time - last_frame_time_).count();
    last_frame_time_ = current_frame_time;
    return delta_time_;
}
}  // namespace renderer
