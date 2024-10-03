#include "fileController.h"

#include <fstream>

namespace FileController {

std::vector<std::uint8_t> readBinaryFile(const std::string &path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    size_t fileSize = (size_t) file.tellg();
    std::vector<std::uint8_t> output(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(output.data()), fileSize);

    file.close();

    return output;
}

}
