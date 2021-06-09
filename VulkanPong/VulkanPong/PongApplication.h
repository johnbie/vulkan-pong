#pragma once
#include "AppObject.h"
#include "RectangleObject.h"
#include "BallObject.h"
#include "PaddleObject.h"
#include "VulkanEngine.h"
#include <functional>

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

    inline static bool _p1UpHold = false;
    inline static bool _p1DownHold = false;
    inline static bool _p2UpHold = false;
    inline static bool _p2DownHold = false;
    
    GLFWwindow* window; // glfw window instance
    VulkanEngine* vulkanEngine; // glfw window instance

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //function generateKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void playBootMusic();
};