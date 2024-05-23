#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

class Renderer {
public:
    Renderer(ShaderProgram* screenShader, ShaderProgram* raytracerShader, GLFWwindow* window, unsigned int * colorBuffer);
    void update();
    void recalculateWorkGroups(int width, int height);

private:
    ShaderProgram* screenShader;
    ShaderProgram* raytracerShader;

    unsigned int * colorBuffer;
    int workgroup_count_x, workgroup_count_y;
};



#endif //RAYTRACING_RENDERER_H
