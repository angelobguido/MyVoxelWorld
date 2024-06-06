#include "Renderer.h"

Renderer::Renderer(ShaderProgram *screenShader, ShaderProgram *raytracerShader, ShaderProgram *accumulatorShader, GLFWwindow *window,
                   unsigned int *colorBuffer,
                   unsigned int *sceneFrameBuffer, unsigned int *accumulationColorBuffer,
                   unsigned int *accumulationFrameBuffer)
        : screenShader(screenShader), raytracerShader(raytracerShader), accumulatorShader(accumulatorShader), colorBuffer(colorBuffer), sceneFrameBuffer(sceneFrameBuffer), accumulationBuffer(accumulationColorBuffer), accumulationFrameBuffer(accumulationFrameBuffer) {

    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    workgroup_count_x = (w + 7) / 8;
    workgroup_count_y = (h + 7) / 8;

    glGenBuffers(1, &accumulationBufferProgressive);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, accumulationBufferProgressive);
    std::vector<float> data(w*h*4, 0); // Initialize accumulation buffer with zero
    glBufferData(GL_SHADER_STORAGE_BUFFER, w*h*sizeof(float)*4, data.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, accumulationBufferProgressive);

    gen = std::mt19937 (rd());
    dist = std::uniform_int_distribution<int>();
    this->window = window;

}

void Renderer::update(Detector &detector) {

    glClear(GL_COLOR_BUFFER_BIT);

    int reset = 0;

    if(detector.getSomethingMoved()){
        accumulationCounter = 1;
        reset = 1;
    }


    glBindImageTexture(0, *colorBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    raytracerShader->use();
    raytracerShader->setInt("rendererRandom", dist(gen));
    glDispatchCompute(workgroup_count_x,workgroup_count_y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    accumulatorShader->use();
    accumulatorShader->setInt("reset", reset);
    accumulatorShader->setInt("numberOfAccumulations", accumulationCounter);
    glDispatchCompute(workgroup_count_x,workgroup_count_y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


    screenShader->use();
    glBindTexture(GL_TEXTURE_2D, *colorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);

    accumulationCounter++;

}

void Renderer::recalculateWorkGroups(int width, int height) {
    workgroup_count_x = (width + 7) / 8;
    workgroup_count_y = (height + 7) / 8;
}

void Renderer::resizeAccumulationBuffer(int width, int height) {
    glGenBuffers(1, &accumulationBufferProgressive);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, accumulationBufferProgressive);
    std::vector<float> data(width*height*4, 0); // Initialize accumulation buffer with zero
    glBufferData(GL_SHADER_STORAGE_BUFFER, width*height*sizeof(float)*4, data.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, accumulationBufferProgressive);
}
