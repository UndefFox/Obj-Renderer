#include "fileController.h"

#include <fstream>
#include <array>
#include <sstream>
#include <cstdlib>

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

std::vector<Vertex> readObjFile(const std::string &path) {
    std::vector<Vertex> output;

    std::ifstream file(path);

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 3>> normals;
    std::vector<std::array<float, 2>> textures;

    if (file.is_open()) {
        std::string line;
        std::string param;

        while (getline(file, line)) {
            std::istringstream params(line);

            getline(params, param, ' ');

            if (param == std::string("v")) {
                std::array<float, 3> pos;

                getline(params, param, ' ');
                pos[0] = std::atof(param.data());
                getline(params, param, ' ');
                pos[1] = std::atof(param.data());
                getline(params, param, ' ');
                pos[2] = std::atof(param.data());
                
                positions.push_back(pos);
            }
            else if (param == std::string("vt")) {
                std::array<float, 2> texture;

                getline(params, param, ' ');
                texture[0] = std::atof(param.data());
                getline(params, param, ' ');
                texture[1] = std::atof(param.data());
                
                textures.push_back(texture);
            }
            else if (param == std::string("vn")) {
                std::array<float, 3> normal;

                getline(params, param, ' ');
                normal[0] = std::atof(param.data());
                getline(params, param, ' ');
                normal[1] = std::atof(param.data());
                getline(params, param, ' ');
                normal[2] = std::atof(param.data());
                
                normals.push_back(normal);
            }
            
            else if (param == std::string("f")) {
                Vertex vertex;
                
                
                std::string face;
                std::string index;
                for (int i = 0; i < 3; i++) {
                    getline(params, face, ' ');
                    std::istringstream indexes(face);

                    getline(indexes, index, '/');
                    std::array<float, 3> pos = positions[std::atoi(index.data()) - 1];
                    vertex.pos[0] = pos[0];
                    vertex.pos[1] = pos[1];
                    vertex.pos[2] = pos[2];

                    getline(indexes, index, '/');
                    if (!index.empty()) {
                        std::array<float, 2> texture = textures[std::atoi(index.data()) - 1];
                        vertex.texCoord[0] = texture[0];
                        vertex.texCoord[1] = texture[1];
                    }

                    getline(indexes, index, '/');
                    if (!index.empty()) {
                        std::array<float, 3> normal = normals[std::atoi(index.data()) - 1];
                        vertex.normal[0] = normal[0];
                        vertex.normal[1] = normal[1];
                        vertex.normal[2] = normal[2];
                    }
                    
                    output.push_back(vertex);
                }
            }
        }
    }

    return output;
}

}
