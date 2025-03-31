#include "file_reader.h"
#include <sstream>
#include <stdexcept>

namespace renderer {
FileReader::FileReader(const std::filesystem::path& filename) : filename_(filename) {
    file_.open(filename);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename.string());
    }
}

std::string FileReader::GetContent() const {
    std::stringstream buffer;
    buffer << file_.rdbuf();
    return buffer.str();
}

bool FileReader::IsOpen() const {
    return file_.is_open();
}

LineParser::LineParser(const std::string& line) : line_(line) {
}

bool LineParser::IsVertexLine() const {
    return !line_.empty() && line_[0] == 'v' && line_[1] == ' ';
}

bool LineParser::IsFaceLine() const {
    return !line_.empty() && line_[0] == 'f' && line_[1] == ' ';
}

std::tuple<double, double, double> LineParser::ExtractCoordinates() const {
    if (!IsVertexLine()) {
        throw std::runtime_error("Not a vertex line");
    }

    std::stringstream vert(line_.substr(2));
    double x, y, z;
    vert >> x >> y >> z;
    return {x, y, z};
}

std::vector<int> LineParser::ExtractIndices() const {
    if (!IsFaceLine()) {
        throw std::runtime_error("Not a face line");
    }

    std::stringstream face(line_.substr(2));
    std::vector<int> indices;
    int idx;
    while (face >> idx) {
        indices.push_back(idx - 1);
    }

    if (indices.size() < 3) {
        throw std::runtime_error("Face must have at least 3 vertices");
    }

    return indices;
}
}  // namespace renderer
