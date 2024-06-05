#include "Detector.h"

Detector::Detector(VoxelWorldBuilder* builder):builder(builder) {}

void Detector::detectCloseAction() {
    shouldClose = true;
}

void Detector::resetMoveDetection() {
    somethingMoved = false;
}

void Detector::detectMovement() {
    if(canDetectMovement)
        somethingMoved = true;
}

void Detector::disableMovementDetection() {
    canDetectMovement = false;
}

void Detector::enableMovementDetection() {
    canDetectMovement = true;
}

bool Detector::getSomethingMoved() const {
    return somethingMoved;
}

bool Detector::getShouldClose() const {
    return shouldClose;
}

// Apply voxel grid ray tracing to detect a block given a position and direction
Detector::BlockDetection Detector::detectBlock(glm::vec3 origin, glm::vec3 direction) {

    BlockDetection blockDetection;
    blockDetection.hit = false;

    direction = glm::normalize(direction);
    glm::ivec3 voxelPosition;
    float t = 0;
    glm::ivec3 normal(0);
    glm::ivec3 gridSize = builder->gridSize;

    glm::vec3 invDir = {1.0 / direction.x, 1.0 / direction.y, 1.0 / direction.z};
    glm::ivec3 stepInGrid(glm::sign(direction));

    voxelPosition = (glm::floor(origin));
    // Check if the current voxel is out of the grid bounds
    if (voxelPosition.x < 0 || voxelPosition.x >= gridSize.x ||
        voxelPosition.y < 0 || voxelPosition.y >= gridSize.y ||
        voxelPosition.z < 0 || voxelPosition.z >= gridSize.z) {
        return blockDetection;
    }

    // Calculate tMax and tDelta
    glm::vec3 tMax = ((glm::vec3(voxelPosition) + (glm::vec3(stepInGrid)*0.5f + 0.5f) ) - origin) * invDir;

    glm::vec3 tDelta = abs(invDir);

    while (t<8) {

        // Check if the current voxel is out of the grid bounds
        if (voxelPosition.x < 0 || voxelPosition.x >= gridSize.x ||
            voxelPosition.y < 0 || voxelPosition.y >= gridSize.y ||
            voxelPosition.z < 0 || voxelPosition.z >= gridSize.z) {
            return blockDetection;
        }

        // Check if the current voxel is a block
        if (builder->voxelGrid[voxelPosition.x + voxelPosition.z * gridSize.x + voxelPosition.y * gridSize.x * gridSize.z] != 0) {
            std::cout<<builder->voxelGrid[voxelPosition.x + voxelPosition.z * gridSize.x + voxelPosition.y * gridSize.x * gridSize.z]<<std::endl;

            blockDetection.hit = true;
            blockDetection.normal = normal;
            blockDetection.position = voxelPosition;

            return blockDetection;
        }

        // Traverse the grid with voxel steps
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                voxelPosition.x += stepInGrid.x;
                t = tMax.x;
                normal = glm::ivec3(-stepInGrid.x, 0, 0);
                tMax.x += tDelta.x;
            } else {
                voxelPosition.z += stepInGrid.z;
                t = tMax.z;
                normal = glm::ivec3(0, 0, -stepInGrid.z);
                tMax.z += tDelta.z;
            }
        } else {
            if (tMax.y < tMax.z) {
                voxelPosition.y += stepInGrid.y;
                t = tMax.y;
                normal = glm::ivec3(0, -stepInGrid.y, 0);
                tMax.y += tDelta.y;
            } else {
                voxelPosition.z += stepInGrid.z;
                t = tMax.z;
                normal = glm::ivec3(0, 0, -stepInGrid.z);
                tMax.z += tDelta.z;
            }
        }

    }

    std::cout<<"No block! 3"<<std::endl;
    return blockDetection;
}

void Detector::detectBlockAndBreak(glm::vec3 origin, glm::vec3 direction) {
    BlockDetection blockDetection = detectBlock(origin, direction);

    if(blockDetection.hit){
        builder->breakBlock(blockDetection.position);
    }

}

void Detector::detectBlockAndPlace(glm::vec3 origin, glm::vec3 direction, int blockType) {

}

