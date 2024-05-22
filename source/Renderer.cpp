#include "Renderer.h"

Renderer::Renderer(ShaderProgram *screenShader, ShaderProgram *raytracerShader, GLFWwindow *window,
                   unsigned int colorBuffer) : screenShader(screenShader), raytracerShader(raytracerShader), colorBuffer(colorBuffer) {

    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    workgroup_count_x = (w + 7) / 8;
    workgroup_count_y = (h + 7) / 8;

}

void Renderer::update() {

    raytracerShader->use();
    glBindImageTexture(0, colorBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(workgroup_count_x,workgroup_count_y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    screenShader->use();
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}