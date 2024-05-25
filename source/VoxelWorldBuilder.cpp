#include "VoxelWorldBuilder.h"

VoxelWorldBuilder::VoxelWorldBuilder(ShaderProgram *raytracer, int gridSizeX, int gridSizeY, int gridSizeZ) : raytracer(raytracer), gridSize({gridSizeX, gridSizeY, gridSizeZ}) {
    voxelGrid = std::vector<int>(gridSizeX*gridSizeY*gridSizeZ, 0);
}

void VoxelWorldBuilder::build() {

    std::random_device rd;

    std::mt19937 gen(rd());

    int min = 0;
    int max = 1;

    std::uniform_int_distribution<int> dist(min, max);

    for(int x = 0; x < gridSize.x; x++){
        for(int y = 0; y < gridSize.y; y++){
            for(int z = 0; z < gridSize.z; z++){
                voxelGrid[x + z*gridSize.x + y*gridSize.x*gridSize.z] = dist(gen);
            }
        }
    }
    std::cout << "Grid created" << std::endl;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, voxelGrid.size()*sizeof(int), voxelGrid.data(), GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);

    raytracer->use();
    raytracer->setIVec3("gridSize", gridSize);

}

VoxelWorldBuilder::~VoxelWorldBuilder() {
    glDeleteBuffers(1, &buffer);
}
