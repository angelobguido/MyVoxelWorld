#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Detector.h"
#include <utility>
#include <random>

class Renderer {

    const int maxAccumulationFrames = 1000;

public:
    Renderer(ShaderProgram *screenShader, ShaderProgram *raytracerShader, ShaderProgram *accumulatorShader, GLFWwindow *window, unsigned int *colorBuffer,
             unsigned int *sceneFrameBuffer, unsigned int *accumulationColorBuffer, unsigned int *accumulationFrameBuffer);
    void update(Detector &detector);
    void recalculateWorkGroups(int width, int height);
    void resizeAccumulationBuffer(int width, int height);

private:
    ShaderProgram* screenShader;
    ShaderProgram* raytracerShader;
    ShaderProgram* accumulatorShader;

    unsigned int * colorBuffer;
    unsigned int *sceneFrameBuffer;
    unsigned int *accumulationBuffer;
    unsigned int *accumulationFrameBuffer;
    unsigned int accumulationBufferProgressive;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

    GLFWwindow* window;

    int workgroup_count_x, workgroup_count_y;
    int accumulationCounter = 0;
};



#endif //RAYTRACING_RENDERER_H
