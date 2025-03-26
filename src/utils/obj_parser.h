#pragma once

#include "color.h"
#include "object.h"
#include <string>
#include <vector>

namespace renderer {
class Parser {
public:
    static renderer::Object LoadObj(const std::string& filename, Color color = kWhiteColor,
                                    const Vector3& translation = kZeroVector3,
                                    const Matrix3& rotation = kIdentityMatrix3);

private:
    static std::vector<Vector3> ParseVertices(const std::string& content);
    static std::vector<Triangle> ParseFaces(const std::string& content,
                                            const std::vector<Vector3>& vertices);
};
}  // namespace renderer
