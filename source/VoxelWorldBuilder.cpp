#include "VoxelWorldBuilder.h"

VoxelWorldBuilder::VoxelWorldBuilder(ShaderProgram *raytracer, int gridSizeX, int gridSizeY, int gridSizeZ, int brickSize) : raytracer(raytracer), gridSize({gridSizeX-gridSizeX%brickSize, gridSizeY-gridSizeY%brickSize, gridSizeZ-gridSizeZ%brickSize}), brickSize(brickSize) {
    brickGrid = std::vector<Brick>((gridSize.x/brickSize)*(gridSize.y/brickSize)*(gridSize.z/brickSize), Brick());
    voxelGrid = std::vector<int>(gridSize.x*gridSize.y*gridSize.z, 0);
}

void VoxelWorldBuilder::build() {

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

    // Create a house
    glm::ivec3 houseEdgePosition(30,50,50);
    glm::ivec3 houseDimension(20,6,10);
    for (int x = 0; x < houseDimension.x; x++) {
        for (int y = 0; y < houseDimension.y; y++) {
            for (int z = 0; z < houseDimension.z; z++) {

                if(x == 0 || ((houseDimension.x-1-x)!=0 && (houseDimension.y-1-y)!=0 && (houseDimension.z-1-z)!=0 && (y!=0) && (z!=0) ))
                    voxelGrid[(x+houseEdgePosition.x) + (z+houseEdgePosition.z) * gridSize.x + (y+houseEdgePosition.y) * gridSize.x * gridSize.z] = 0;
                else
                    voxelGrid[(x+houseEdgePosition.x) + (z+houseEdgePosition.z) * gridSize.x + (y+houseEdgePosition.y) * gridSize.x * gridSize.z] = 1;
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

void VoxelWorldBuilder::changeBlock(glm::ivec3 position, int blockType) {
    voxelGrid[position.x + position.z * gridSize.x + position.y * gridSize.x * gridSize.z] = blockType;

    int brickSizeX = gridSize.x/brickSize;
    int brickSizeY = gridSize.y/brickSize;
    int brickSizeZ = gridSize.z/brickSize;

    Brick& brick = brickGrid[(position.x/brickSize) + (position.z/brickSize) * brickSizeX + (position.y/brickSize) * brickSizeX * brickSizeZ];

    glm::ivec3 positionInsideBrick = position%brickSize;

    if(brick.type == -1){
        voxelInBrickGrid[brick.offSet + (positionInsideBrick.x) + (positionInsideBrick.z) * brickSize + (positionInsideBrick.y) * brickSize * brickSize] = blockType;
    }else{
        int lastType = brick.type;
        brick.type = -1;
        brick.offSet = voxelInBrickGrid.size();

        for (int x = 0; x < brickSize; x++) {
            for (int y = 0; y < brickSize; y++) {
                for (int z = 0; z < brickSize; z++) {

                    voxelInBrickGrid[brick.offSet + x + z * brickSize + y * brickSize * brickSize] = lastType;
                    if((positionInsideBrick.x == x) && (positionInsideBrick.y == y) && (positionInsideBrick.z == z))
                        voxelInBrickGrid[brick.offSet + x + z * brickSize + y * brickSize * brickSize] = blockType;
                }
            }
        }

    }

    updateBuffers();
}

void VoxelWorldBuilder::updateBuffers() {
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

    std::cout << "Buffers updated" << std::endl;
}
