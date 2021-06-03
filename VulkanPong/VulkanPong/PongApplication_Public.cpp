// public functions
#include "PongApplication.h"

void PongApplication::run() {
    initVulkan();
    mainLoop();
    cleanup();
}
