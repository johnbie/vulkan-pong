#pragma once
#include "PongApplication.h"

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
    int gameObjectCount = 3;
    Vertex* vertices = vulkanEngine->AllocateVerticesAndIndicesForRectangles(gameObjectCount);
    
    // instantiate objects
    p1 = new RectangleObject(vertices, 16, 92, 2, 20, true);
    p2 = new RectangleObject(vertices+4, 222, 92, 2, 20, true);
    ball = new RectangleObject(vertices+8, 119, 99, 2, 2, true);

    // set up vulkan
    vulkanEngine->Initialize();
}

void PongApplication::update()
{
    // continuous polling. alternative is glfwWaitEvents, which waits for events to occur
    glfwPollEvents();

    // set positions and indices
    p1->Update();
    p2->Update();
    ball->Update();

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