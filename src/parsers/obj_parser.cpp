#include "obj_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "color.h"

namespace renderer {
std::string ObjParser::ReadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool ObjParser::IsVertexLine(const std::string& line) {
    return !line.empty() && line[0] == 'v' && line[1] == ' ';
}

bool ObjParser::IsFaceLine(const std::string& line) {
    return !line.empty() && line[0] == 'f' && line[1] == ' ';
}

std::tuple<double, double, double> ObjParser::ExtractCoordinates(const std::string& vertex_data) {
    std::stringstream vert(vertex_data);
    double x, y, z;
    vert >> x >> y >> z;
    return {x, y, z};
}

std::vector<int> ObjParser::ExtractIndices(const std::string& face_data) {
    std::stringstream face(face_data);
    std::vector<int> indices;
    int idx;
    while (face >> idx) {
        indices.push_back(idx - 1);
    }
    return indices;
}

std::vector<Object> ObjParser::CreateObjects(
    const std::vector<std::pair<std::string, Color>>& file_color_mappings,
    const Vector3& translation, const Matrix3& rotation) {

    if (file_color_mappings.empty()) {
        return {};
    }

    std::vector<Object> objects;

    for (const auto& mapping : file_color_mappings) {
        std::string content = ReadFile(mapping.first);

        auto vertices = ParseVertices(content);
        if (vertices.empty()) {
            return {};
        }

        auto triangles = ParseFaces(content, vertices, mapping.second);
        if (triangles.empty()) {
            return {};
        }

        objects.emplace_back(std::move(triangles), translation, rotation);
    }

    return objects;
}

std::vector<Vector3> ObjParser::ParseVertices(const std::string& content) {
    std::vector<Vector3> vertices;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        if (!IsVertexLine(line)) {
            continue;
        }

        auto [x, y, z] = ExtractCoordinates(line.substr(2));
        vertices.emplace_back(x, y, z);
    }

    return vertices;
}

std::vector<Triangle> ObjParser::ParseFaces(const std::string& content,
                                            const std::vector<Vector3>& vertices, Color color) {
    if (vertices.empty()) {
        return {};
    }

    std::vector<Triangle> triangles;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        if (!IsFaceLine(line)) {
            continue;
        }

        auto indices = ExtractIndices(line.substr(2));

        if (indices.size() < 3) {
            continue;
        }

        for (size_t i = 1; i < indices.size() - 1; ++i) {
            if (indices[0] < 0 || static_cast<size_t>(indices[0]) >= vertices.size() ||
                indices[i] < 0 || static_cast<size_t>(indices[i]) >= vertices.size() ||
                indices[i + 1] < 0 || static_cast<size_t>(indices[i + 1]) >= vertices.size()) {
                continue;
            }

            Vector3 a = vertices[indices[0]];
            Vector3 b = vertices[indices[i]];
            Vector3 c = vertices[indices[i + 1]];
            auto calculate_normal = [](const Vector3& a, const Vector3& b, const Vector3& c) {
                return (b - a).cross(c - a).normalized();
            };
            triangles.emplace_back(a, b, c, calculate_normal(a, b, c), color);
        }
    }

    return triangles;
}
}  // namespace renderer
