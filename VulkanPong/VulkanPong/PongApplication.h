#pragma once
#include "AppObject.h"
#include "RectangleObject.h"
#include "BallObject.h"
#include "PaddleObject.h"
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
    int _time;
    PaddleObject* p1;
    PaddleObject* p2;
    BallObject* ball;
    
    GLFWwindow* window; // glfw window instance
    VulkanEngine* vulkanEngine; // glfw window instance
};