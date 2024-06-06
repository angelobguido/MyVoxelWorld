#include "App.h"

App* App::currentApp = nullptr;

App::App(int gridSizeX, int gridSizeY, int gridSizeZ):gridSizeX(gridSizeX),gridSizeY(gridSizeY),gridSizeZ(gridSizeZ) {
    setUpGlfw();
    currentApp = this;
}

App::~App() {
    delete raytracerShader;
    delete screenShader;
    delete accumulatorShader;

    glDeleteTextures(1, &colorBuffer);

    glfwTerminate();

    delete renderer;
    delete builder;
}

void App::run() {

    glfwShowWindow(window);

    setUpCallbacks();
    builder->build();

    Detector detector(builder);

    lastTime = glfwGetTime();
    lastTimeForFrame = lastTime;
    numFrames = 0;
    frameTime = 16.0f;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        detector.resetMoveDetection();

        handleFrameTiming();
        camera->update(delta, detector);
        if (detector.getShouldClose()) {
            break;
        }

        renderer->update(detector);

    }
}

void App::setUpOpenGl() {

    // Init glew to use the OpenGL api.
    uint GlewInitResult = glewInit();
    printf("GlewStatus: %s", glewGetErrorString(GlewInitResult));

    // Adjust the viewport size using the width and height of the glfw window.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0,0,width,height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    // Create the shaders, the screen shader is used to draw the screen texture and the raytracer shader is used to create the screen texture with the ray tracing techniques.
    screenShader = new ShaderProgram({{"./shaders/fragment.frag", GL_FRAGMENT_SHADER}, {"./shaders/vertex.vert", GL_VERTEX_SHADER}});
    raytracerShader = new ShaderProgram({{"./shaders/raytracer.comp", GL_COMPUTE_SHADER}});
    accumulatorShader = new ShaderProgram({{"./shaders/accumulator.comp", GL_COMPUTE_SHADER}});

    // Generate and bind framebuffers
    glGenFramebuffers(1, &sceneFramebuffer);
    glGenFramebuffers(1, &accumulationFrameBuffer);

    // Generate textures
    glGenTextures(1, &colorBuffer);
    glGenTextures(1, &accumulationColorBuffer);

    // Setup scene draw texture
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach scene draw texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: "<<status<<std::endl;

    // Setup accumulation texture
    glBindTexture(GL_TEXTURE_2D, accumulationColorBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach accumulation texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, accumulationFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumulationColorBuffer, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: "<<status<<std::endl;

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void App::handleFrameTiming() {
    currentTime = (float)glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;

    float deltaForFrame = currentTime - lastTimeForFrame;

    if (deltaForFrame >= 1) {
        int framerate{ std::max(1, int(numFrames / deltaForFrame)) };
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window, title.str().c_str());
        lastTimeForFrame = currentTime;
        numFrames = -1;
        frameTime = float(1000.0 / framerate);
    }

    ++numFrames;
}

void App::setUpGlfw() {
    //Create glfw window
    glfwInit();

    window = glfwCreateWindow(1250, 1000, "Minha Janela", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void App::createRenderer() {
    renderer = new Renderer(screenShader, raytracerShader, accumulatorShader, window, &colorBuffer, &sceneFramebuffer, &accumulationColorBuffer, &accumulationFrameBuffer);
}

void App::createCamera() {
    camera = new Camera(raytracerShader, window);
}

void App::setUpCallbacks() {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void App::framebuffer_size_callback(GLFWwindow *window, int width, int height) {

    float quality = currentApp->quality;
    glViewport(0,0,width,height);
    width = int(width*quality);
    height = int(height*quality);

    currentApp->recreateColorBuffer(width,height);
    currentApp->camera->recalculateProjection(width, height);
    currentApp->renderer->recalculateWorkGroups(width, height);
    currentApp->renderer->resizeAccumulationBuffer(width, height);

}

void App::recreateColorBuffer(int width, int height) {

    glDeleteTextures(1, &colorBuffer);
    glDeleteTextures(1, &accumulationColorBuffer);

    // Generate textures
    glGenTextures(1, &colorBuffer);
    glGenTextures(1, &accumulationColorBuffer);

    // Setup scene draw texture
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach scene draw texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: "<<status<<std::endl;

    // Setup accumulation texture
    glBindTexture(GL_TEXTURE_2D, accumulationColorBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach accumulation texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, accumulationFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumulationColorBuffer, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: "<<status<<std::endl;

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void App::setUpBuilder() {
    builder = new VoxelWorldBuilder(raytracerShader, gridSizeX, gridSizeY, gridSizeZ, 8);
}

