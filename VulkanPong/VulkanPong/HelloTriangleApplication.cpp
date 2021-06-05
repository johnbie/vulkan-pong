#pragma once
#include "HelloTriangleApplication.h"

// public functions
HelloTriangleApplication::HelloTriangleApplication(uint32_t width, uint32_t height, bool initialize) : WIDTH(width), HEIGHT(height)
{
    if (initialize)
        this->initialize();
    else
    {
        vulkanEngine = nullptr;
        window = nullptr;
    }
}

void HelloTriangleApplication::Run()
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

void HelloTriangleApplication::initialize()
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
    
    //initVulkan();
    vulkanEngine = new VulkanEngine(window, true);
}

void HelloTriangleApplication::update()
{
    // continuous polling. alternative is glfwWaitEvents, which waits for events to occur
    glfwPollEvents();

    vulkanEngine->Update();
}

void HelloTriangleApplication::clean()
{
    vulkanEngine->Clean();

    // clean glfw and window
    glfwDestroyWindow(window);
    glfwTerminate();
}