#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Renderer.h"

class App {
public:
    App();
    ~App();
    void run();
    void setUpOpenGl();
    void createRenderer();

private:
    void setUpGlfw();
    void handleFrameTiming();

    Renderer* renderer;

    GLFWwindow* window;
    ShaderProgram* raytracerShader;
    ShaderProgram* screenShader;
    unsigned int colorBuffer;

    double lastTime, currentTime;
    int numFrames;
    float frameTime;

};


#endif //RAYTRACING_APP_H
