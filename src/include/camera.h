#pragma once

class Camera {
public:
    Camera(double fov, double aspect, double near, double far);

private:
    double fov_;
    double aspect_;
    double near_;
    double far_;
};
