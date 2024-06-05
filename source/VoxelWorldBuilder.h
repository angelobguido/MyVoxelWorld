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
    VoxelWorldBuilder(ShaderProgram* raytracer, int gridSizeX, int gridSizeY, int gridSizeZ, int brickSize);
    ~VoxelWorldBuilder();
    void build();
    void breakBlock(glm::ivec3 position);
    void placeBlock(glm::ivec3 position, int blockType);

    std::vector<Brick> brickGrid;
    std::vector<int> voxelInBrickGrid;
    std::vector<int> voxelGrid;
    glm::ivec3 gridSize;
    ShaderProgram* raytracer;
    unsigned int buffer;
    unsigned int voxelsInBrickBuffer;
    unsigned int brickBuffer;
private:
    void updateBuffers();

    int brickSize;

};


#endif //RAYTRACING_VOXELWORLDBUILDER_H
