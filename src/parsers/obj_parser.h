#pragma once

#include "color.h"
#include "object.h"
#include <string>
#include <vector>

namespace renderer {
class ObjParser {
public:
    static std::vector<Object> CreateObjects(
        const std::vector<std::pair<std::string, Color>>& filenames,
        const Vector3& translation = kZeroVector3, const Matrix3& rotation = kIdentityMatrix3);

private:
    static std::vector<Vector3> ParseVertices(const std::string& content);
    static std::vector<Triangle> ParseFaces(const std::string& content,
                                            const std::vector<Vector3>& vertices, Color color);
};
}  // namespace renderer
