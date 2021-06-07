#pragma once
//#include "HelloTriangleApplication.h"
#include "PongApplication.h"

#include <iostream>

// code runner
int main() {
    //HelloTriangleApplication app(WIDTH, HEIGHT);
    PongApplication app;

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}