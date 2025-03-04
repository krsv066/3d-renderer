#pragma once

#include "object.h"
#include <string>
#include <vector>
#include <Eigen/Dense>

namespace renderer::parser {
class Parser {
public:
    static renderer::Object LoadObj(const std::string& filename,
                                    const Eigen::Vector3d& translation = Eigen::Vector3d::Zero(),
                                    const Eigen::Matrix3d& rotation = Eigen::Matrix3d::Identity(),
                                    uint32_t color = 0xFFFFFF);

private:
    static std::vector<Eigen::Vector3d> ParseVertices(const std::string& content);
    static std::vector<primitive::Triangle> ParseFaces(const std::string& content,
                                                       const std::vector<Eigen::Vector3d>& vertices,
                                                       uint32_t color);
};
}  // namespace renderer::parser
