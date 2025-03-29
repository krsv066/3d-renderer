#pragma once

#include "color.h"
#include <string>
#include <vector>

namespace renderer {
class CmdParser {
public:
    static std::vector<std::pair<std::string, Color>> ExtractFromArgs(int argc, char* argv[]);
};
}  // namespace renderer
