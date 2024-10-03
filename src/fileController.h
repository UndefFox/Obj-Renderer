#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace FileController {

std::vector<std::uint8_t> readBinaryFile(const std::string &path);

}