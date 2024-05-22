#include "App.h"

App::App() {
    setUpGlfw();
}

App::~App() {
    delete raytracerShader;
    delete screenShader;

    glDeleteTextures(1, &colorBuffer);

    glfwTerminate();

    delete renderer;
}

void App::run() {

    glfwShowWindow(window);

    lastTime = glfwGetTime();
    numFrames = 0;
    frameTime = 16.0f;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        renderer->update();

        glfwSwapBuffers(window);

        handleFrameTiming();

    }
}

void App::setUpOpenGl() {

    uint GlewInitResult = glewInit();
    printf("GlewStatus: %s", glewGetErrorString(GlewInitResult));

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0,0,width,height);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    screenShader = new ShaderProgram({{"./shaders/fragment.frag", GL_FRAGMENT_SHADER}, {"./shaders/vertex.vert", GL_VERTEX_SHADER}});
    raytracerShader = new ShaderProgram({{"./shaders/raytracer.comp", GL_COMPUTE_SHADER}});

    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
}

void App::handleFrameTiming() {
    currentTime = glfwGetTime();
    double delta = currentTime - lastTime;

    if (delta >= 1) {
        int framerate{ std::max(1, int(numFrames / delta)) };
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window, title.str().c_str());
        lastTime = currentTime;
        numFrames = -1;
        frameTime = float(1000.0 / framerate);
    }

    ++numFrames;
}

void App::setUpGlfw() {
    glfwInit();

    window = glfwCreateWindow(800, 800, "Minha Janela", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
}

void App::createRenderer() {
    renderer = new Renderer(screenShader, raytracerShader, window, colorBuffer);
}