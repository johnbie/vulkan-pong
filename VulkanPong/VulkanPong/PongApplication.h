#pragma once
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class PongApplication {
public:
    void run();
private:
    void initVulkan();
    void mainLoop();
    void cleanup();
};