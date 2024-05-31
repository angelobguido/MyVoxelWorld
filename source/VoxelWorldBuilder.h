#ifndef RAYTRACING_VOXELWORLDBUILDER_H
#define RAYTRACING_VOXELWORLDBUILDER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <random>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "PerlinNoise/PerlinNoise.hpp"

class VoxelWorldBuilder {

    struct Brick{
        int offSet;
        int type;
    };

public:
    std::vector<Brick> brickGrid;
    std::vector<int> voxelInBrickGrid;
    glm::ivec3 gridSize;
    ShaderProgram* raytracer;
    VoxelWorldBuilder(ShaderProgram* raytracer, int gridSizeX, int gridSizeY, int gridSizeZ, int brickSize);
    ~VoxelWorldBuilder();
    void build();
    unsigned int buffer;
    unsigned int voxelsInBrickBuffer;
    unsigned int brickBuffer;
private:
    int brickSize;

};


#endif //RAYTRACING_VOXELWORLDBUILDER_H
