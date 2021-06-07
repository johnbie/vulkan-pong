#pragma once
#include "AppObject.h"
#include "RectangleObject.h"
#include "VulkanEngine.h"

// #include "pch.h"

// c++ libraries
#include <vector>

class PongApplication : public AppObject
{
public:
    PongApplication(bool initialize = false);
    void Run();
protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    RectangleObject* p1;
    RectangleObject* p2;
    RectangleObject* ball;
    
    GLFWwindow* window; // glfw window instance
    VulkanEngine* vulkanEngine; // glfw window instance
};