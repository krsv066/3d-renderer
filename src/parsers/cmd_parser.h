#pragma once

#include "color.h"
#include "linalg.h"
#include <vector>
#include <filesystem>

namespace renderer {
struct ModelInfo {
    std::filesystem::path path;
    Color color = Color::White();
    Vector3 position = Vector3::Zero();
};

class CmdParser {
public:
    static std::vector<ModelInfo> ExtractModelInfo(int argc, char* argv[]);

private:
    static std::pair<Vector3, int> ParsePosition(int argc, char* argv[], int index);
};
}  // namespace renderer
