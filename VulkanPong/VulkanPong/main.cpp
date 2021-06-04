#pragma once
#include "PongApplication.h"

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// code runner
int main() {
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