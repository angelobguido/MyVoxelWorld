#include "Camera.h"

Camera::Camera(ShaderProgram *raytracerShader, GLFWwindow *window): raytracerShader(raytracerShader), window(window) {

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    recalculateProjection(width, height);
    recalculateView();
}

bool Camera::update(float deltaTime) {

    glm::vec3 right = glm::normalize(glm::cross(front, up));

    //Keys
    glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
    float dUp = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dPos.x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dPos.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dPos.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dPos.y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        dUp += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        dUp -= 1.0f;
    }

    if (glm::length(dPos) > 0.1f) {
        dPos = glm::normalize(dPos);
        position += deltaTime * dPos.x * front * speed;
        position += deltaTime * dPos.y * right * speed;
    }

    if (glm::abs(dUp) > 0.1f){
        position.y += deltaTime * dUp * speed;
    }

    //Commands:

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return true;
    }


    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        fullscreen = true;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && fullscreen) {
        glfwSetWindowMonitor(window, nullptr, 100, 100, WIDTH, HEIGHT, GLFW_DONT_CARE);
        fullscreen = false;
    }


    //Mouse
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse){
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    double xOffSet = mouseX - lastX;
    double yOffSet = lastY - mouseY;
    lastX = mouseX;
    lastY = mouseY;

    xOffSet *= sensitivity;
    yOffSet *= sensitivity;


    yaw += xOffSet;
    pitch += yOffSet;

    if (pitch > 89.0)
        pitch = 89.0;
    if (pitch < -89.0)
        pitch = -89.0;

    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(front);

    recalculateView();
    raytracerShader->use();
    raytracerShader->setMat4("inverseViewMatrix", inverseViewMatrix);
    raytracerShader->setMat4("inverseProjectionMatrix", inverseProjectionMatrix);
    raytracerShader->setVec3("cameraPosition", position);

//    std::cout<<position.x<<" "<<position.y<<" "<<position.z<<std::endl;

    return false;
}


void Camera::recalculateView() {
    inverseViewMatrix = glm::inverse(glm::lookAt(position, position + front, up));
}

void Camera::recalculateProjection(int width, int height) {
    inverseProjectionMatrix = glm::inverse(glm::perspectiveFov(fov, (float)width, (float)height, near, far));
}

