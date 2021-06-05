#pragma once
#include "pch.h"

#include "AppObject.h"
#include "VulkanEngine.h"
#include "VulkanEngineHelper.h"
#include "DebugHelper.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include "Vertex.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>

class HelloTriangleApplication : public AppObject
{
public:
    HelloTriangleApplication(uint32_t width, uint32_t height, bool initialize = false);
    void Run();

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    const uint32_t WIDTH, HEIGHT;
    GLFWwindow* window; // glfw window instance
    VulkanEngine* vulkanEngine; // glfw window instance
};