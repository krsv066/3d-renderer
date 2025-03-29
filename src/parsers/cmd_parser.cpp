#include "cmd_parser.h"
#include <cstdint>
#include <stdexcept>
#include "color.h"

namespace renderer {
std::vector<std::pair<std::string, Color>> CmdParser::ExtractFromArgs(int argc, char* argv[]) {
    std::vector<std::pair<std::string, Color>> paths;

    if (argc == 1) {
        throw std::runtime_error("To few arguments");
    }

    for (int i = 1; i < argc;) {
        std::string model_path = std::string("../models/") + argv[i] + ".obj";
        Color color = kWhiteColor;

        if (i + 1 < argc) {
            try {
                color = Color(static_cast<uint32_t>(std::stoul(argv[i + 1], nullptr, 0)));
                i += 2;
            } catch (const std::exception&) {
                ++i;
            }
        } else {
            ++i;
        }

        paths.push_back({model_path, color});
    }

    return paths;
}
}  // namespace renderer