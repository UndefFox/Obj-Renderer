#include "shapeGenerator.h"

namespace ShapeGenerator {

std::vector<Vertex> box(float length, float width, float height) {

    float xOffset = length / (length + height);
    float yOffestDowm = width / (width + height) / 2;
    float yOffestUp = height / (width + height) / 2;

    return std::vector<Vertex>{
        // X+ face
        Vertex{.pos = { length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {1, yOffestDowm}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {xOffset, yOffestDowm}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {1, 2*yOffestDowm}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {xOffset, yOffestDowm}}, 
        Vertex{.pos = { length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {xOffset, 2*yOffestDowm}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {1, 0, 0}, .texCoord = {1, 2*yOffestDowm}},
        // X- face
        Vertex{.pos = {-length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {1, 0}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {xOffset, yOffestDowm}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {1, yOffestDowm}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {1, 0}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {xOffset, 0}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {-1, 0, 0}, .texCoord = {xOffset, yOffestDowm}},
        // Y+ face
        Vertex{.pos = {-length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {0, 1}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {0, 1 - yOffestUp}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {xOffset, 1 - yOffestUp}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {xOffset, 1 - yOffestUp}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {xOffset, 1}}, 
        Vertex{.pos = {-length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 1, 0}, .texCoord = {0, 1}},
        // Y- face
        Vertex{.pos = { length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {xOffset, 1 - yOffestUp}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {0, 1 - yOffestUp}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {0, 1 - 2*yOffestUp}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {0, 1 - 2*yOffestUp}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {xOffset, 1 - 2*yOffestUp}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, -1, 0}, .texCoord = {xOffset, 1 - yOffestUp}},
        // Z+ face
        Vertex{.pos = { length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {xOffset, 0}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {xOffset, yOffestDowm}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {0, yOffestDowm}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {0, yOffestDowm}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {0, 0}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f,  height / 2.0f}, .normal = {0, 0, 1}, .texCoord = {xOffset, 0}},
        // Z- face
        Vertex{.pos = {-length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {0, yOffestDowm}},
        Vertex{.pos = { length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {xOffset, yOffestDowm}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {xOffset, 2*yOffestDowm}},
        Vertex{.pos = { length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {xOffset, 2*yOffestDowm}},
        Vertex{.pos = {-length / 2.0f, -width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {0, 2*yOffestDowm}},
        Vertex{.pos = {-length / 2.0f,  width / 2.0f, -height / 2.0f}, .normal = {0, 0, -1}, .texCoord = {0, yOffestDowm}}
    };
}

}