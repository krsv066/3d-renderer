#include "obj_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "object.h"

namespace renderer::parser {
renderer::Object Parser::LoadObj(const std::string& filename, const Eigen::Vector3d& translation,
                                 const Eigen::Matrix3d& rotation, uint32_t color) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto vertices = ParseVertices(content);
    auto triangles = ParseFaces(content, vertices, color);
    renderer::Object obj{triangles, translation, rotation};

    return renderer::Object{triangles, translation, rotation};
}

std::vector<Eigen::Vector3d> Parser::ParseVertices(const std::string& content) {
    std::vector<Eigen::Vector3d> vertices;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty() || line[0] != 'v' || line[1] != ' ') {
            continue;
        }

        std::stringstream vert(line.substr(2));
        double x, y, z;
        vert >> x >> y >> z;
        vertices.emplace_back(x, y, z);
    }

    return vertices;
}

std::vector<primitive::Triangle> Parser::ParseFaces(const std::string& content,
                                                    const std::vector<Eigen::Vector3d>& vertices,
                                                    uint32_t color) {
    std::vector<primitive::Triangle> triangles;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty() || line[0] != 'f' || line[1] != ' ') {
            continue;
        }

        std::stringstream face(line.substr(2));
        std::vector<int> indices;
        int idx;
        while (face >> idx) {
            indices.push_back(idx - 1);
        }

        for (size_t i = 1; i < indices.size() - 1; ++i) {
            triangles.push_back(
                {vertices[indices[0]], vertices[indices[i]], vertices[indices[i + 1]], color});
        }
    }

    return triangles;
}
}  // namespace renderer::parser
