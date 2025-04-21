#pragma once

#include "cmd_parser.h"
#include "color.h"
#include "object.h"
#include <string>
#include <vector>

namespace renderer {
class ObjParser {
public:
    static std::vector<Object> CreateObjects(const std::vector<ModelInfo>& model_infos, const Vector3& translation = Vector3::Zero(), const Matrix3& rotation = Matrix3::Identity());

private:
    static Object LoadObj(const ModelInfo& model_info, const Vector3& translation, const Matrix3& rotation);
    static Vector3 CalculateNormal(const Vector3& a, const Vector3& b, const Vector3& c);
    static std::vector<Vector3> ParseVertices(const std::string& content);
    static std::vector<Triangle> ParseFaces(const std::string& content, const std::vector<Vector3>& vertices, Color color);
    static bool AreIndicesValid(const std::vector<int>& indices, const std::vector<Vector3>& vertices, size_t i);
};
}  // namespace renderer
