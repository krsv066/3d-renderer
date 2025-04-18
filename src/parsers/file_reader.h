#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace renderer {
class FileReader {
public:
    explicit FileReader(const std::filesystem::path& filename);
    std::string GetContent() const;
    bool IsOpen() const;

private:
    std::ifstream file_;
    std::filesystem::path filename_;
};

class LineParser {
public:
    explicit LineParser(const std::string& line);
    bool IsVertexLine() const;
    bool IsFaceLine() const;
    std::tuple<double, double, double> ExtractCoordinates() const;
    std::vector<int> ExtractIndices() const;

private:
    std::string line_;
};
}  // namespace renderer
