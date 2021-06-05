#pragma once
//#include "HelloTriangleApplication.h"
#include "PongApplication.h"

#include <iostream>

const uint32_t WIDTH = 1200;
const uint32_t HEIGHT = 1000;


// code runner
int main() {
    //HelloTriangleApplication app(WIDTH, HEIGHT);
    PongApplication app(WIDTH, HEIGHT);

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}