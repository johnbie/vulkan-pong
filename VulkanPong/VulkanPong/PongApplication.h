#pragma once
#include "AppObject.h"
#include "VulkanEngine.h"

// #include "pch.h"

// c++ libraries
#include <vector>

class PongApplication : public AppObject
{
public:
    PongApplication(uint32_t width, uint32_t height, bool initialize = false);
    void Run();
protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    const uint32_t WIDTH, HEIGHT;
    const uint32_t PIXEL_SIZE = 10;
    GLFWwindow* window; // glfw window instance
    VulkanEngine* vulkanEngine; // glfw window instance
};