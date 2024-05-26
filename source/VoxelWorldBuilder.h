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

public:
    std::vector<int> voxelGrid;
    glm::ivec3 gridSize;
    ShaderProgram* raytracer;
    VoxelWorldBuilder(ShaderProgram* raytracer, int gridSizeX, int gridSizeY, int gridSizeZ);
    ~VoxelWorldBuilder();
    void build();
    unsigned int buffer;

};


#endif //RAYTRACING_VOXELWORLDBUILDER_H
