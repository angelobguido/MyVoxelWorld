#include "Camera.h"

Camera::Camera(ShaderProgram *raytracerShader, GLFWwindow *window): raytracerShader(raytracerShader), window(window) {

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    recalculateProjection(width, height);
    recalculateView();
}

void Camera::update(float deltaTime, Detector &detector) {

    glm::vec3 right = glm::normalize(glm::cross(front, up));

    // Handle cooldown
    actionTimer -= deltaTime;

    // Handle camera movement
    {
        glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
        float dUp = 0;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            dPos.x += 1.0f; // Straight
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            dPos.y -= 1.0f; // Left
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            dPos.x -= 1.0f; // Back
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            dPos.y += 1.0f; // Right
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            dUp += 1.0f; // Up
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            dUp -= 1.0f; // Down
        }

        // Update camera position if the total movement is big enough
        if (glm::length(dPos) > 0.1f) {
            dPos = glm::normalize(dPos);
            position += deltaTime * dPos.x * front * speed;
            position += deltaTime * dPos.y * right * speed;

            detector.detectMovement();
        }

        if (glm::abs(dUp) > 0.1f) {
            position.y += deltaTime * dUp * speed;

            detector.detectMovement();
        }
    }

    // Handle object movement
    {
        glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
        float dUp = 0;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            dPos.z -= 1.0f; // Straight
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            dPos.x -= 1.0f; // Left
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            dPos.z += 1.0f; // Back
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            dPos.x += 1.0f; // Right
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            dUp += 1.0f; // Up
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            dUp -= 1.0f; // Down
        }

        // Update object position if the total movement is big enough
        if (glm::length(dPos) > 0.1f) {
            dPos = glm::normalize(dPos);

            switch (selectedObject) {
                case 1: object1Position += deltaTime * dPos * speed;
                        break;
                case 2: object2Position += deltaTime * dPos * speed;
                        break;
            }

            detector.detectMovement();
        }

        if (glm::abs(dUp) > 0.1f) {
            switch (selectedObject) {
                case 1: object1Position.y += deltaTime * dUp * speed;
                    break;
                case 2: object2Position.y += deltaTime * dUp * speed;
                    break;
            }

            detector.detectMovement();
        }
    }


    // Handle commands
    {
        // Exit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            detector.detectCloseAction();
        }

        // Fullscreen
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fullscreen) {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            fullscreen = true;
        }

        // Windowed
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && fullscreen) {
            glfwSetWindowMonitor(window, nullptr, 100, 100, WIDTH, HEIGHT, GLFW_DONT_CARE);
            fullscreen = false;
        }
    }

    // Handle player actions
    {
        // Break block
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            if(actionTimer < 0){
                // Reset timer
                actionTimer = actionCooldownTime;
                detector.detectBlockAndBreak(position, front);
                detector.detectMovement();
            }
        }

        // Place block
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            if(actionTimer < 0){
                // Reset timer
                actionTimer = actionCooldownTime;
                detector.detectBlockAndPlace(position, front, selectedBlock);
                detector.detectMovement();
            }
        }

        // Choose block
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
            selectedBlock = 1;
            detector.detectMovement();
        }

        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
            selectedBlock = 2;
            detector.detectMovement();
        }

        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
            selectedBlock = 3;
            detector.detectMovement();
        }

        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
            selectedBlock = 4;
            detector.detectMovement();
        }

        // Choose object
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            selectedObject = 1;
        }

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            selectedObject = 2;
        }


        // Increment and decrement ambient light
        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            detector.detectMovement();
            ambientLightPower += increment * deltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            detector.detectMovement();
            ambientLightPower -= increment * deltaTime;
        }


        // Change material of selected block type diffuse and specular properties
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            detector.detectMovement();
            roughnessControl += increment*deltaTime;
            if(roughnessControl > 1){
                roughnessControl = 1;
            }
        }

        if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
            detector.detectMovement();
            roughnessControl -= increment*deltaTime;
            if(roughnessControl < 0){
                roughnessControl = 0;
            }
        }

    }


    // Handle mouse movements
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (firstMouse) {
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

        if (std::abs(xOffSet) > 0.1 || std::abs(yOffSet) > 0.1)
            detector.detectMovement();


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
    }

    // Update shader uniforms with current matrices.
    recalculateView();
    raytracerShader->use();
    raytracerShader->setMat4("inverseViewMatrix", inverseViewMatrix);
    raytracerShader->setMat4("inverseProjectionMatrix", inverseProjectionMatrix);
    raytracerShader->setVec3("cameraPosition", position);
    raytracerShader->setVec3("object1Position", object1Position);
    raytracerShader->setVec3("object2Position", object2Position);
    raytracerShader->setFloat("ambientLightPower", ambientLightPower);
    raytracerShader->setFloat("roughnessControl", roughnessControl);
    raytracerShader->setInt("selectedBlock", selectedBlock);

}


void Camera::recalculateView() {
    inverseViewMatrix = glm::inverse(glm::lookAt(position, position + front, up));
}

void Camera::recalculateProjection(int width, int height) {
    inverseProjectionMatrix = glm::inverse(glm::perspectiveFov(fov, (float)width, (float)height, near, far));
}

