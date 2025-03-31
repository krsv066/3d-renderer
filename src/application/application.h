#pragma once

#include "drawer.h"
#include "renderer.h"
#include "timer.h"
#include <string>
#include <memory>

namespace renderer {
class Application {
public:
    Application(int argc, char* argv[]);
    void Run();

private:
    Movement ProcessInput(double delta_time);

    static constexpr Width kDefaultWidth{1280};
    static constexpr Height kDefaultHeight{720};
    static constexpr double kDistanceScale = 2.5;
    static constexpr double kRotationScale = 1.0;
    static constexpr std::string kDefaultTitel = "3D Renderer";
    Renderer renderer_;
    World world_;
    Camera camera_;
    Timer timer_;
    Drawer drawer_;
    std::unique_ptr<Screen> screen_;
};
}  // namespace renderer
