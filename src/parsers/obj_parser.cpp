#include "color.h"
#include "file_reader.h"
#include "obj_parser.h"
#include <sstream>
#include <stdexcept>

namespace renderer {
std::vector<Object> ObjParser::CreateObjects(const std::vector<ModelInfo>& model_infos, const Vector3& translation, const Matrix3& rotation) {
    if (model_infos.empty()) {
        return {};
    }

    std::vector<Object> objects;

    for (const auto& model : model_infos) {
        try {
            Vector3 final_position = translation + model.position;
            objects.push_back(LoadObj(model, final_position, rotation));
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to load model " + model.path.string() + ": " + e.what());
        }
    }

    return objects;
}

Object ObjParser::LoadObj(const ModelInfo& model_info, const Vector3& translation, const Matrix3& rotation) {
    FileReader reader(model_info.path);
    std::string content = reader.GetContent();

    auto vertices = ParseVertices(content);
    if (vertices.empty()) {
        throw std::runtime_error("No vertices found in model");
    }

    auto triangles = ParseFaces(content, vertices, model_info.color);
    if (triangles.empty()) {
        throw std::runtime_error("No faces found in model");
    }

    return Object(std::move(triangles), translation, rotation);
}

Vector3 ObjParser::CalculateNormal(const Vector3& a, const Vector3& b, const Vector3& c) {
    return (b - a).cross(c - a).normalized();
}

std::vector<Vector3> ObjParser::ParseVertices(const std::string& content) {
    std::vector<Vector3> vertices;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        LineParser parser(line);
        if (!parser.IsVertexLine()) {
            continue;
        }

        auto [x, y, z] = parser.ExtractCoordinates();
        vertices.emplace_back(x, y, z);
    }

    return vertices;
}

bool ObjParser::AreIndicesValid(const std::vector<int>& indices, const std::vector<Vector3>& vertices, size_t i) {
    return indices[0] >= 0 && static_cast<size_t>(indices[0]) < vertices.size() && indices[i] >= 0 && static_cast<size_t>(indices[i]) < vertices.size() && indices[i + 1] >= 0 &&
           static_cast<size_t>(indices[i + 1]) < vertices.size();
}

std::vector<Triangle> ObjParser::ParseFaces(const std::string& content, const std::vector<Vector3>& vertices, Color color) {
    if (vertices.empty()) {
        return {};
    }

    std::vector<Triangle> triangles;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        LineParser parser(line);
        if (!parser.IsFaceLine()) {
            continue;
        }

        try {
            auto indices = parser.ExtractIndices();

            for (size_t i = 1; i < indices.size() - 1; ++i) {
                if (!AreIndicesValid(indices, vertices, i)) {
                    continue;
                }

                Vector3 a = vertices[indices[0]];
                Vector3 b = vertices[indices[i]];
                Vector3 c = vertices[indices[i + 1]];
                Matrix3 verts;
                verts << a.x(), b.x(), c.x(), a.y(), b.y(), c.y(), a.z(), b.z(), c.z();
                triangles.emplace_back(verts, CalculateNormal(a, b, c), color);
            }
        } catch (const std::exception&) {
            continue;
        }
    }

    return triangles;
}
}  // namespace renderer
