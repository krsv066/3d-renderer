#include "renderer.h"
#include "object.h"
#include "world.h"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

Renderer::Renderer() {
    z_buffer_.assign(width_ * height_, std::numeric_limits<double>::infinity());
    frame_buffer_.assign(width_ * height_ * 4, 0);
}

void Renderer::SetWindow(uint32_t width, uint32_t height) {
    width_ = width;
    height_ = height;
}

void Renderer::SetProjectionMatrix(double fov, double aspect, double near, double far) {
    const double tan_half_fov = std::tan(fov / 2.0);
    projection_matrix_ = Eigen::Matrix4d::Zero();
    projection_matrix_(0, 0) = 1.0 / (aspect * tan_half_fov);
    projection_matrix_(1, 1) = 1.0 / tan_half_fov;
    projection_matrix_(2, 2) = -(far + near) / (far - near);
    projection_matrix_(2, 3) = -(2.0 * far * near) / (far - near);
    projection_matrix_(3, 2) = -1.0;
}

void Renderer::Render(const World& scene) {
    Rasterize(scene);
    Show();
}

void Renderer::Rasterize(const World& scene) {
    for (const auto& obj : scene.objects_) {
        for (const auto& tr : obj.object_) {
            const Eigen::Vector4d p0 = ProjectVertex(tr.a);
            const Eigen::Vector4d p1 = ProjectVertex(tr.b);
            const Eigen::Vector4d p2 = ProjectVertex(tr.c);
            const uint32_t color = tr.color;

            /* clang-format off */
            const int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x(), p1.x(), p2.x()}))));
            const int max_x = std::min(static_cast<int>(width_) - 1, static_cast<int>(std::ceil(std::max({p0.x(), p1.x(), p2.x()}))));
            const int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y(), p1.y(), p2.y()}))));
            const int max_y = std::min(static_cast<int>(height_) - 1, static_cast<int>(std::ceil(std::max({p0.y(), p1.y(), p2.y()}))));
            /* clang-format on */

            double area = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y());

            for (int y = min_y; y <= max_y; ++y) {
                for (int x = min_x; x <= max_x; ++x) {
                    double w0 = EdgeFunction(p1.x(), p1.y(), p2.x(), p2.y(), x, y);
                    double w1 = EdgeFunction(p2.x(), p2.y(), p0.x(), p0.y(), x, y);
                    double w2 = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), x, y);

                    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;

                        const double z = w0 * p0.z() + w1 * p1.z() + w2 * p2.z();

                        const int index = GetBufferIndex(x, y);
                        if (z < z_buffer_[index]) {
                            z_buffer_[index] = z;

                            const size_t pixel_index = index * 4;
                            frame_buffer_[pixel_index] = (color >> 16) & 0xFF;
                            frame_buffer_[pixel_index + 1] = (color >> 8) & 0xFF;
                            frame_buffer_[pixel_index + 2] = color & 0xFF;
                            frame_buffer_[pixel_index + 3] = 0xFF;
                        }
                    }
                }
            }
        }
    }
}

void Renderer::Show() const {
    sf::RenderWindow window(sf::VideoMode({width_, height_}), "3d renderer");
    sf::Texture texture(sf::Vector2u(width_, height_));
    texture.update(frame_buffer_.data());
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

Eigen::Vector4d Renderer::ProjectVertex(const Eigen::Vector3d& p) const {
    Eigen::Vector4d pos(p.x(), p.y(), p.z(), 1.0);
    Eigen::Vector4d projected = projection_matrix_ * pos;

    if (projected.w() != 0) {
        projected.x() /= projected.w();
        projected.y() /= projected.w();
        projected.z() /= projected.w();
    }

    projected.x() = (projected.x() + 1.0) * width_ * 0.5;
    projected.y() = (projected.y() + 1.0) * height_ * 0.5;

    return projected;
}

int Renderer::GetBufferIndex(int x, int y) const {
    return y * width_ + x;
}

double Renderer::EdgeFunction(double x0, double y0, double x1, double y1, double x,
                              double y) const {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}
