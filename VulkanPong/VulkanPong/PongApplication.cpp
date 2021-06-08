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

void PongApplication::initialize()
{
    // initialze the glfw instance
    glfwInit();

    // set up window configs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window not resizable

    // create and set window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Pong", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); // callback for resizing
    
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