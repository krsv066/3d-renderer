#pragma once

#include "color.h"
#include "object.h"
#include <string>
#include <vector>

namespace renderer::parser {
class Parser {
public:
    static renderer::Object LoadObj(const std::string& filename, Color color = kWhiteColor,
                                    const linalg::Vector3& translation = linalg::kZeroVector3,
                                    const linalg::Matrix3& rotation = linalg::kIdentityMatrix3);

private:
    static std::vector<linalg::Vector3> ParseVertices(const std::string& content);
    static std::vector<primitive::Triangle> ParseFaces(
        const std::string& content, const std::vector<linalg::Vector3>& vertices);
};
}  // namespace renderer::parser
