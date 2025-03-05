#pragma once

#include "object.h"
#include <string>
#include <vector>

namespace renderer::parser {
class Parser {
public:
    static renderer::Object LoadObj(const std::string& filename,
                                    const Vector3& translation = Vector3::Zero(),
                                    const Matrix3& rotation = Matrix3::Identity(),
                                    uint32_t color = 0xFFFFFF);

private:
    static std::vector<Vector3> ParseVertices(const std::string& content);
    static std::vector<primitive::Triangle> ParseFaces(const std::string& content,
                                                       const std::vector<Vector3>& vertices,
                                                       uint32_t color);
};
}  // namespace renderer::parser
