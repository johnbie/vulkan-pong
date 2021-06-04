// public functions
#include "PongApplication.h"

PongApplication::PongApplication(uint32_t width, uint32_t height, bool initialize) : WIDTH(width), HEIGHT(height)
{
    if (initialize)
        this->initialize();
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
    // set up vulkan
    vulkanEngine = new VulkanEngine(true);

    // initialze the glfw instance
    glfwInit();

    // set up window configs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window not resizable

    // create and set window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Pong", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); // callback for resizing
}

void PongApplication::update()
{
    // continuous polling. alternative is glfwWaitEvents, which waits for events to occur
    glfwPollEvents();

    // update something here
    
    // set positions and indices

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