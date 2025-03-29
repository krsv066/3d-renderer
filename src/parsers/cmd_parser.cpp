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

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc) {
            paths.push_back({std::string("../models/") + argv[i] + ".obj",
                             Color(static_cast<uint32_t>(std::stoul(argv[i + 1], nullptr, 0)))});
        } else {
            throw std::runtime_error("Color value is missing for model " + std::string(argv[i]));
        }
    }

    return paths;
}
}  // namespace renderer