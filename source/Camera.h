#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Detector.h"
#include <iostream>
#include <algorithm>

class Camera {

    const float speed = 10;
    const float sensitivity = 0.2;
    const float near = 0.01;
    const float far = 1000;
    const float actionCooldownTime = 0.1;

    const int WIDTH = 800;
    const int HEIGHT = 600;

    const float increment = 1;

public:
    Camera(ShaderProgram* raytracerShader, GLFWwindow* window);

    void update(float deltaTime, Detector &detector);

    void move(float deltaTime, int key);
    void look(double mouseX, double mouseY);
    void zoom();

    void recalculateProjection(int width, int height);
    void recalculateView();

private:

    GLFWwindow * window;
    ShaderProgram * raytracerShader;

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

    glm::vec3 position = glm::vec3(5,120,5);
    glm::vec3 front = glm::vec3(0,0,-1);
    glm::vec3 up = glm::vec3(0,1,0);
    float fov = 45;
    double lastX = 800.0/2;
    double lastY = 600.0/2;
    bool firstMouse = true;
    double yaw = -90;
    double pitch = 0;
    bool fullscreen = false;

    float actionTimer = 0;

    float roughnessControl = 0;
    float ambientLightPower = 0;

    int selectedBlock = 4;

    glm::vec3 object1Position = glm::vec3(50, 50, 70);
    glm::vec3 object2Position = glm::vec3(50, 50, 70);

    int selectedObject = 1;

    glm::mat4 inverseProjectionMatrix;
    glm::mat4 inverseViewMatrix;

};


#endif //RAYTRACING_CAMERA_H
