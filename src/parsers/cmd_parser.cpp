#include "cmd_parser.h"
#include "color.h"
#include <cstdint>
#include <stdexcept>

namespace renderer {
std::pair<Vector3, int> CmdParser::ParsePosition(int argc, char* argv[], int index) {
    Vector3 position = Vector3::Zero();
    int updated_index = index;

    if (index + 2 < argc) {
        char* x_end = nullptr;
        char* y_end = nullptr;
        char* z_end = nullptr;

        double x = std::strtod(argv[index], &x_end);
        double y = std::strtod(argv[index + 1], &y_end);
        double z = std::strtod(argv[index + 2], &z_end);

        if (*x_end == '\0' && *y_end == '\0' && *z_end == '\0') {
            position = Vector3(x, y, z);
            updated_index += 3;
        }
    }

    return {position, updated_index};
}

std::vector<ModelInfo> CmdParser::ExtractModelInfo(int argc, char* argv[]) {
    std::vector<ModelInfo> models;

    if (argc == 1) {
        throw std::runtime_error("Too few arguments");
    }

    for (int i = 1; i < argc;) {
        std::filesystem::path model_path = std::filesystem::path("..") / "models" / (std::string(argv[i]) + ".obj");
        Color color = Color::White();
        Vector3 position = Vector3::Zero();

        ++i;
        if (i < argc) {
            char* end_ptr = nullptr;
            int64_t value = std::strtoul(argv[i], &end_ptr, 0);

            if (*end_ptr == '\0') {
                color = Color(static_cast<uint32_t>(value));
                ++i;

                auto [parsed_position, updated_index] = ParsePosition(argc, argv, i);
                position = parsed_position;
                i = updated_index;
            }
        }

        models.emplace_back(model_path, color, position);
    }

    return models;
}
}  // namespace renderer
