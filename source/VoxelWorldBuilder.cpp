#include "VoxelWorldBuilder.h"

VoxelWorldBuilder::VoxelWorldBuilder(ShaderProgram *raytracer, int gridSizeX, int gridSizeY, int gridSizeZ, int brickSize) : raytracer(raytracer), gridSize({gridSizeX-gridSizeX%brickSize, gridSizeY-gridSizeY%brickSize, gridSizeZ-gridSizeZ%brickSize}), brickSize(brickSize) {
    brickGrid = std::vector<Brick>((gridSize.x/brickSize)*(gridSize.y/brickSize)*(gridSize.z/brickSize), Brick());
}

void VoxelWorldBuilder::build() {

    std::vector<int> voxelGrid(gridSize.x*gridSize.y*gridSize.z, 0);

    std::random_device rd;
    std::mt19937 gen(rd());

    int min = 0;
    int max = 3;

    std::uniform_int_distribution<int> dist(min, max);
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};


    for (int x = 0; x < gridSize.x; x++) {
        for (int y = 0; y < gridSize.y; y++) {
            for (int z = 0; z < gridSize.z; z++) {
                const int noise = (int) ((perlin.noise2D((x * 0.01), (z * 0.01)) * 0.5 + 0.5) * gridSize.y);
                if (y == noise){
                    voxelGrid[x + z * gridSize.x + y * gridSize.x * gridSize.z] = 2;
                    if(x%8==0&&z%8==0)
                        voxelGrid[x + z * gridSize.x + y * gridSize.x * gridSize.z] = 3;
                }

                if (x==gridSize.x/2 && z==gridSize.z/2){
                    voxelGrid[x + z * gridSize.x + y * gridSize.x * gridSize.z] = 4;
                }

                else if (y < noise) {
                    voxelGrid[x + z * gridSize.x + y * gridSize.x * gridSize.z] = 1;
                }



            }
        }
    }

    int brickSizeX = gridSize.x/brickSize;
    int brickSizeY = gridSize.y/brickSize;
    int brickSizeZ = gridSize.z/brickSize;

    for (int brickX = 0; brickX < brickSizeX; brickX++) {
        for (int brickY = 0; brickY < brickSizeY; brickY++) {
            for (int brickZ = 0; brickZ < brickSizeZ; brickZ++) {

                std::vector<int> brickVoxels(brickSize*brickSize*brickSize, 0);
                bool sameVoxels = true;
                int lastVoxel = voxelGrid[(brickX*brickSize) + (brickZ*brickSize) * gridSize.x + (brickY*brickSize) * gridSize.x * gridSize.z];

                for (int x = 0; x < brickSize; x++) {
                    for (int y = 0; y < brickSize; y++) {
                        for (int z = 0; z < brickSize; z++) {

                            brickVoxels[x + z * brickSize + y * brickSize * brickSize] = voxelGrid[(brickX*brickSize+x) + (brickZ*brickSize+y) * gridSize.x + (brickY*brickSize+z) * gridSize.x * gridSize.z];
                            if(brickVoxels[x + z * brickSize + y * brickSize * brickSize] != lastVoxel){
                                sameVoxels = false;
                            }
                            lastVoxel = brickVoxels[x + z * brickSize + y * brickSize * brickSize];

                        }
                    }
                }

                if(sameVoxels){
                    brickGrid[brickX + brickZ * brickSizeX + brickY * brickSizeX * brickSizeZ].type = lastVoxel;
                }else{
                    brickGrid[brickX + brickZ * brickSizeX + brickY * brickSizeX * brickSizeZ].type = -1;
                    brickGrid[brickX + brickZ * brickSizeX + brickY * brickSizeX * brickSizeZ].offSet = voxelInBrickGrid.size();

                    for(auto voxel : brickVoxels){
                        voxelInBrickGrid.push_back(voxel);
                    }

                }

            }
        }
    }

//    for(int x = 0; x < gridSize.x; x++){
//        for(int y = 0; y < gridSize.y; y++){
//            for(int z = 0; z < gridSize.z; z++){
//
//                voxelGrid[x + z*gridSize.x + y*gridSize.x*gridSize.z] = dist(gen);
//
//            }
//        }
//    }

    std::cout << "Grid created" << std::endl;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, voxelGrid.size()*sizeof(int), voxelGrid.data(), GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);

    glGenBuffers(1, &brickBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, brickBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, brickGrid.size()*sizeof(Brick), brickGrid.data(), GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, brickBuffer);

    glGenBuffers(1, &voxelsInBrickBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxelsInBrickBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, voxelInBrickGrid.size()*sizeof(int), voxelInBrickGrid.data(), GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, voxelsInBrickBuffer);

    raytracer->use();
    raytracer->setIVec3("gridSize", gridSize);

}

VoxelWorldBuilder::~VoxelWorldBuilder() {
    glDeleteBuffers(1, &buffer);
}
