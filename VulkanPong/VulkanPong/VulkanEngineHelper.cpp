#pragma once
#include "VulkanEngineHelper.h"
#include "Debug.h"

const std::vector<const char*> VulkanEngineHelper::validationLayers = {
    "VK_LAYER_KHRONOS_validation", // useful standard validations
};

const std::vector<const char*> VulkanEngineHelper::deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME // swapchains
};

// gets and returns the required extensions
std::vector<const char*> VulkanEngineHelper::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (Debug::ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // extension only needed in debug mode
    }

    return extensions;
}

bool VulkanEngineHelper::checkValidationLayerSupport()
{
    // prepare layer-getting
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    // get all the layers
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // run through each requested layers
    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        // look for the requested layer
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        // requested layer not found
        if (!layerFound) {
            return false;
        }
    }

    // all requested layers found
    return true;
}