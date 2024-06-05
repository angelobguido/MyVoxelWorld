#ifndef RAYTRACING_DETECTOR_H
#define RAYTRACING_DETECTOR_H

#include "VoxelWorldBuilder.h"
#include <glm/glm.hpp>
#include <iostream>

class Detector {

    struct BlockDetection{
        bool hit;
        glm::ivec3 position;
        glm::ivec3 normal;
    };

public:
    Detector(VoxelWorldBuilder* builder);

    void detectCloseAction();
    void resetMoveDetection();
    void detectMovement();
    void disableMovementDetection();
    void enableMovementDetection();
    bool getSomethingMoved() const;
    bool getShouldClose() const;
    void detectBlockAndBreak(glm::vec3 origin, glm::vec3 direction);
    void detectBlockAndPlace(glm::vec3 origin, glm::vec3 direction, int blockType);

private:
    BlockDetection detectBlock(glm::vec3 origin, glm::vec3 direction);
    bool shouldClose = false;
    bool somethingMoved = false;
    bool canDetectMovement = true;
    VoxelWorldBuilder* builder;

};


#endif //RAYTRACING_DETECTOR_H
