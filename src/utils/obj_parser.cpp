#include "obj_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace renderer::parser {
renderer::Object Parser::LoadObj(const std::string& filename, const Vector3& translation,
                                 const Matrix3& rotation, uint32_t color) {
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

std::vector<Vector3> Parser::ParseVertices(const std::string& content) {
    std::vector<Vector3> vertices;
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
                                                    const std::vector<Vector3>& vertices,
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
