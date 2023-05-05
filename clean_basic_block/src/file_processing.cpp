#include "file_processing.h"

// Read Binary Data From File
std::vector<uint8_t> GetBinaryData(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return {};
    }
    std::vector<uint8_t> b_code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return b_code;
}