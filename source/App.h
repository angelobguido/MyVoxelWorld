#ifndef RAYTRACING_APP_H
#define RAYTRACING_APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"


class App {
public:
    App();
    ~App();
    void run();
    void setUpOpenGl();
    void createRenderer();
    void createCamera();

    static App* currentApp;

private:
    void setUpGlfw();
    void handleFrameTiming();

    void setUpCallbacks();
    void recreateColorBuffer(int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    Renderer* renderer;
    Camera* camera;

    GLFWwindow* window;
    ShaderProgram* raytracerShader;
    ShaderProgram* screenShader;
    unsigned int colorBuffer;

    double lastTime, currentTime;
    int numFrames;
    float frameTime;

};


#endif //RAYTRACING_APP_H
