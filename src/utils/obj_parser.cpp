#include "obj_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

namespace renderer {
renderer::Object Parser::LoadObj(const std::string& filename, Color color,
                                 const Vector3& translation, const Matrix3& rotation) {
    assert(!filename.empty());
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto vertices = ParseVertices(content);
    assert(!vertices.empty());
    auto triangles = ParseFaces(content, vertices);
    assert(!triangles.empty());
    renderer::Object obj(std::move(triangles), translation, rotation, color);

    return obj;
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

std::vector<Triangle> Parser::ParseFaces(const std::string& content,
                                         const std::vector<Vector3>& vertices) {
    assert(!vertices.empty());
    std::vector<Triangle> triangles;
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
            assert(idx > 0);
            assert(static_cast<size_t>(idx) <= vertices.size());
            indices.push_back(idx - 1);
        }

        assert(indices.size() >= 3);
        for (size_t i = 1; i < indices.size() - 1; ++i) {
            triangles.push_back(
                {vertices[indices[0]], vertices[indices[i]], vertices[indices[i + 1]]});
        }
    }

    return triangles;
}
}  // namespace renderer
