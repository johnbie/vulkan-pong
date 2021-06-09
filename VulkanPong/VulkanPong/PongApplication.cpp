#pragma once
#include "PongApplication.h"
#include <chrono>

// public functions
PongApplication::PongApplication(bool initialize)
{
    if (initialize)
        this->Initialize();
    else 
    {
        vulkanEngine = nullptr;
        window = nullptr;
    }
}

void PongApplication::Run()
{
    // instantialize the variables for PongApplication 
    // does nothing if application was already initialized
    Initialize(); 

    // loop
    while (!glfwWindowShouldClose(window))
    {
        // private update call because we know pong is already live
        update();
    }

    Clean(); // clean window and other objects
}

void PongApplication::playBootMusic()
{
    beep(55, 80);
    beep(110, 80);
    beep(220, 80);
    beep(440, 80);
    beep(880, 80);
    beep(1760, 80);
}

void PongApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        PongApplication::_p1UpHold = true;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        PongApplication::_p1UpHold = false;
    }
    
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        PongApplication::_p1DownHold = true;
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        PongApplication::_p1DownHold = false;
    }
    
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        PongApplication::_p2UpHold = true;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    {
        PongApplication::_p2UpHold = false;
    }
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        PongApplication::_p2DownHold = true;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    {
        PongApplication::_p2DownHold = false;
    }
}

void PongApplication::initialize()
{
    // play boot music
    std::thread BEEP(playBootMusic);
    BEEP.detach();

    // initialze the glfw instance
    glfwInit();

    // set up window configs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window not resizable

    // create and set window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Pong", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); // callback for resizing
    
    // set key callback
    glfwSetKeyCallback(window, keyCallback);

    // create vulkan object
    vulkanEngine = new VulkanEngine(window);

    // set up vertices for rectangle objects
    // need 1 for the ball, 2 for the two paddles, and 16 for score-tracking
    int gameObjectCount = 22;
    Vertex* vertices = vulkanEngine->AllocateVerticesAndIndicesForRectangles(gameObjectCount);
    
    // instantiate objects
    p1 = new PaddleObject(vertices, true, true);
    p2 = new PaddleObject(vertices+4, false, true);
    ball = new BallObject(vertices+8, true);

    // score text
    new RectangleObject(vertices + 12, (PIXEL_WIDTH / 4) - 6, 4, 2, 8, true);
    new RectangleObject(vertices + 16, (PIXEL_WIDTH / 4) - 6, 10, 2, 10, true);
    new RectangleObject(vertices + 20, (PIXEL_WIDTH / 4) + 3, 4, 6, 2, true);
    new RectangleObject(vertices + 24, (PIXEL_WIDTH / 4) + 3, 10, 6, 2, true);
    new RectangleObject(vertices + 28, (PIXEL_WIDTH / 4) + 3, 18, 6, 2, true);
    new RectangleObject(vertices + 32, (PIXEL_WIDTH / 4) + 3, 4, 2, 8, true);
    new RectangleObject(vertices + 36, (PIXEL_WIDTH / 4) + 3, 10, 2, 10, true);
    new RectangleObject(vertices + 40, (PIXEL_WIDTH / 4) + 7, 4, 2, 8, true);
    new RectangleObject(vertices + 44, (PIXEL_WIDTH / 4) + 7, 10, 2, 10, true);

    // score text
    new RectangleObject(vertices + 48, (PIXEL_WIDTH / 4 * 3) - 6, 4, 2, 8, true);
    new RectangleObject(vertices + 52, (PIXEL_WIDTH / 4 * 3) - 6, 10, 2, 10, true);
    new RectangleObject(vertices + 56, (PIXEL_WIDTH / 4 * 3) + 3, 4, 6, 2, true);
    new RectangleObject(vertices + 60, (PIXEL_WIDTH / 4 * 3) + 3, 10, 6, 2, true);
    new RectangleObject(vertices + 64, (PIXEL_WIDTH / 4 * 3) + 3, 18, 6, 2, true);
    new RectangleObject(vertices + 68, (PIXEL_WIDTH / 4 * 3) + 3, 4, 2, 8, true);
    new RectangleObject(vertices + 72, (PIXEL_WIDTH / 4 * 3) + 3, 10, 2, 10, true);
    new RectangleObject(vertices + 76, (PIXEL_WIDTH / 4 * 3) + 7, 4, 2, 8, true);
    new RectangleObject(vertices + 80, (PIXEL_WIDTH / 4 * 3) + 7, 10, 2, 10, true);
    new RectangleObject(vertices + 84, PIXEL_WIDTH / 2, 0, 1, PIXEL_HEIGHT, true);

    // set up vulkan
    vulkanEngine->Initialize();
}

void PongApplication::update()
{
    // continuous polling. alternative is glfwWaitEvents, which waits for events to occur
    glfwPollEvents();

    static auto startTime = std::chrono::high_resolution_clock::now(); // start time

    // current time
    auto currentTime = std::chrono::high_resolution_clock::now();
    int time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count() * 15;
    
    if (_time != time)
    {
        _time = time;

        // set positions and indices
        p1->move(PongApplication::_p1UpHold, PongApplication::_p1DownHold);
        p2->move(PongApplication::_p2UpHold, PongApplication::_p2DownHold);
        p1->Update();
        p2->Update();
        ball->Update();
    }

    // update vulkan
    vulkanEngine->Update();
}

void PongApplication::clean()
{
    // clean vulkan
    vulkanEngine->Clean();

    // clean glfw and window
    glfwDestroyWindow(window);
    glfwTerminate();
}