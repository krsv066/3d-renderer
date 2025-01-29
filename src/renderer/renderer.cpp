#include "renderer.h"
#include "object.h"
#include "world.h"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

Renderer::Renderer(const Camera& camera, const World& scene) : camera_(camera), scene_(scene) {
}

void Renderer::SetWindow(uint32_t width, uint32_t height) {
    width_ = width;
    height_ = height;
    z_buffer_.assign(width_ * height_, std::numeric_limits<double>::infinity());
    frame_buffer_.assign(width_ * height_ * 4, 0);
}

void Renderer::SetRenderMode(RenderMode mode) {
    render_mode_ = mode;
}

void Renderer::Render() {
    RenderFrame();
    ShowFrame();
}

void Renderer::RenderFrame() {
    for (const auto& obj : scene_.objects_) {
        RenderObject(obj);
    }
}

void Renderer::RenderObject(const Object& obj) {
    for (const auto& triangle : obj.object_) {
        RenderTriangle(obj, triangle);
    }
}

void Renderer::RenderTriangle(const Object& obj, const Triangle& triangle) {
    const Eigen::Vector4d p0 = ProjectVertex(GetGlobalCoordinates(obj, triangle.a));
    const Eigen::Vector4d p1 = ProjectVertex(GetGlobalCoordinates(obj, triangle.b));
    const Eigen::Vector4d p2 = ProjectVertex(GetGlobalCoordinates(obj, triangle.c));

    if (render_mode_ == RenderMode::WIREFRAME) {
        RenderTriangleWireframe(p0, p1, p2, triangle.color);
        return;
    }

    const int min_x = std::max(0, static_cast<int>(std::floor(std::min({p0.x(), p1.x(), p2.x()}))));
    const int max_x = std::min(static_cast<int>(width_) - 1,
                               static_cast<int>(std::ceil(std::max({p0.x(), p1.x(), p2.x()}))));
    const int min_y = std::max(0, static_cast<int>(std::floor(std::min({p0.y(), p1.y(), p2.y()}))));
    const int max_y = std::min(static_cast<int>(height_) - 1,
                               static_cast<int>(std::ceil(std::max({p0.y(), p1.y(), p2.y()}))));

    const double area = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y());

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            ProcessPixel(x, y, p0, p1, p2, area, triangle.color);
        }
    }
}

void Renderer::RenderTriangleWireframe(const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                                       const Eigen::Vector4d& p2, uint32_t color) {
    DrawLine(p0.x(), p0.y(), p1.x(), p1.y(), color);
    DrawLine(p1.x(), p1.y(), p2.x(), p2.y(), color);
    DrawLine(p2.x(), p2.y(), p0.x(), p0.y(), color);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        if (steep) {
            if (y >= 0 && y < static_cast<int>(width_) && x >= 0 && x < static_cast<int>(height_)) {
                const int index = GetBufferIndex(y, x) * 4;
                frame_buffer_[index] = (color >> 16) & 0xFF;
                frame_buffer_[index + 1] = (color >> 8) & 0xFF;
                frame_buffer_[index + 2] = color & 0xFF;
                frame_buffer_[index + 3] = 0xFF;
            }
        } else {
            if (x >= 0 && x < static_cast<int>(width_) && y >= 0 && y < static_cast<int>(height_)) {
                const int index = GetBufferIndex(x, y) * 4;
                frame_buffer_[index] = (color >> 16) & 0xFF;
                frame_buffer_[index + 1] = (color >> 8) & 0xFF;
                frame_buffer_[index + 2] = color & 0xFF;
                frame_buffer_[index + 3] = 0xFF;
            }
        }

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Renderer::ProcessPixel(int x, int y, const Eigen::Vector4d& p0, const Eigen::Vector4d& p1,
                            const Eigen::Vector4d& p2, double area, uint32_t color) {
    double w0 = EdgeFunction(p1.x(), p1.y(), p2.x(), p2.y(), x, y);
    double w1 = EdgeFunction(p2.x(), p2.y(), p0.x(), p0.y(), x, y);
    double w2 = EdgeFunction(p0.x(), p0.y(), p1.x(), p1.y(), x, y);

    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        UpdatePixel(x, y, w0 / area, w1 / area, w2 / area, p0, p1, p2, color);
    }
}

void Renderer::UpdatePixel(int x, int y, double w0, double w1, double w2, const Eigen::Vector4d& p0,
                           const Eigen::Vector4d& p1, const Eigen::Vector4d& p2, uint32_t color) {
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

void Renderer::ShowFrame() const {
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
    Eigen::Vector4d projected = camera_.GetProjectionMatrix() * pos;

    if (projected.w() != 0) {
        projected.x() /= projected.w();
        projected.y() /= projected.w();
        projected.z() /= projected.w();
    }

    projected.x() = (projected.x() + 1.0) * width_ * 0.5;
    projected.y() = (projected.y() + 1.0) * height_ * 0.5;

    return projected;
}

Eigen::Vector3d Renderer::GetGlobalCoordinates(const Object& obj, const Eigen::Vector3d& p) const {
    return obj.rotation_ * p + obj.translation_;
}

int Renderer::GetBufferIndex(int x, int y) const {
    return (height_ - y) * width_ + x;
}

double Renderer::EdgeFunction(double x0, double y0, double x1, double y1, double x,
                              double y) const {
    return (y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}
