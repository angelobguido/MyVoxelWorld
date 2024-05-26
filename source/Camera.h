#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <iostream>

class Camera {

    const float speed = 10;
    const float sensitivity = 0.1;
    const float near = 0.01;
    const float far = 1000;

    const int WIDTH = 800;
    const int HEIGHT = 600;

public:
    Camera(ShaderProgram* raytracerShader, GLFWwindow* window);

    bool update(float deltaTime);

    void move(float deltaTime, int key);
    void look(double mouseX, double mouseY);
    void zoom();

    void recalculateProjection(int width, int height);
    void recalculateView();

    bool cameraMoved = true;

private:

    GLFWwindow * window;
    ShaderProgram * raytracerShader;

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

    glm::vec3 position = glm::vec3(100,120,100);
    glm::vec3 front = glm::vec3(0,0,-1);
    glm::vec3 up = glm::vec3(0,1,0);
    float fov = 45;
    double lastX = 800.0/2;
    double lastY = 600.0/2;
    bool firstMouse = true;
    double yaw = -90;
    double pitch = 0;
    bool fullscreen = false;

    glm::mat4 inverseProjectionMatrix;
    glm::mat4 inverseViewMatrix;

};


#endif //RAYTRACING_CAMERA_H
