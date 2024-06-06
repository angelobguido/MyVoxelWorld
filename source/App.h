#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"
#include "VoxelWorldBuilder.h"
#include "Detector.h"


class App {
public:
    App(int gridSizeX, int gridSizeY, int gridSizeZ);
    ~App();
    void run();
    void setUpOpenGl();
    void setUpBuilder();
    void createRenderer();
    void createCamera();

    static App* currentApp;
    float quality = 0.5;

private:
    void setUpGlfw();
    void handleFrameTiming();

    void setUpCallbacks();
    void recreateColorBuffer(int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    Renderer* renderer;
    Camera* camera;
    VoxelWorldBuilder* builder;

    GLFWwindow* window;
    ShaderProgram* raytracerShader;
    ShaderProgram* screenShader;
    ShaderProgram* accumulatorShader;
    unsigned int colorBuffer;
    unsigned int accumulationColorBuffer;
    unsigned int sceneFramebuffer;
    unsigned int accumulationFrameBuffer;

    float lastTime, currentTime, lastTimeForFrame;
    int numFrames;
    float frameTime;
    float delta;

    int gridSizeX, gridSizeY, gridSizeZ;

};


#endif //RAYTRACING_APP_H
