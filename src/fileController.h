#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "graphics/vertex.h"

namespace FileController {

std::vector<std::uint8_t> readBinaryFile(const std::string &path);

std::vector<Vertex> readObjFile(const std::string &path);

}