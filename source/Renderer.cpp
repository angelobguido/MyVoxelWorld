#include "Renderer.h"

Renderer::Renderer(ShaderProgram *screenShader, ShaderProgram *raytracerShader, GLFWwindow *window,
                   unsigned int *colorBuffer,
                   unsigned int *sceneFrameBuffer, unsigned int *accumulationColorBuffer,
                   unsigned int *accumulationFrameBuffer)
        : screenShader(screenShader), raytracerShader(raytracerShader), colorBuffer(colorBuffer), sceneFrameBuffer(sceneFrameBuffer), accumulationBuffer(accumulationColorBuffer), accumulationFrameBuffer(accumulationFrameBuffer) {

    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    workgroup_count_x = (w + 7) / 8;
    workgroup_count_y = (h + 7) / 8;

    gen = std::mt19937 (rd());
    dist = std::uniform_int_distribution<int>();
    this->window = window;

}

void Renderer::update(bool cameraMoved) {

    if(cameraMoved){
        glBindFramebuffer(GL_FRAMEBUFFER, *accumulationFrameBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        accumulationCounter = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, *sceneFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    raytracerShader->use();
    raytracerShader->setInt("rendererRandom", dist(gen));
    glBindImageTexture(0, *colorBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(workgroup_count_x,workgroup_count_y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, *accumulationFrameBuffer);
    glEnable(GL_BLEND);

    if (cameraMoved) {
        // Direct copy if the camera has moved recently
        glBlendFunc(GL_ONE, GL_ZERO);
    } else {
        glBlendColor(1,1,1,1/float(accumulationCounter));
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    }

    screenShader->use();
    glBindTexture(GL_TEXTURE_2D, *colorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader->use();
    glBindTexture(GL_TEXTURE_2D, *accumulationBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);

    accumulationCounter++;

    glfwSwapBuffers(window);

}

void Renderer::recalculateWorkGroups(int width, int height) {
    workgroup_count_x = (width + 7) / 8;
    workgroup_count_y = (height + 7) / 8;
}
