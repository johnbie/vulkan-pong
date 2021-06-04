#pragma once
#include "VulkanEngineHelper.h"
#include "DebugHelper.h"

#include <set>

const std::vector<const char*> VulkanEngineHelper::validationLayers = {
    "VK_LAYER_KHRONOS_validation", // useful standard validations
};

const std::vector<const char*> VulkanEngineHelper::deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME // swapchains
};

// gets and returns the required extensions
std::vector<const char*> VulkanEngineHelper::RequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (DebugHelper::ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // extension only needed in debug mode
    }

    return extensions;
}

bool VulkanEngineHelper::ValidationLayersAreSupported()
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

// does the specific device support all the extensions we need?
bool VulkanEngineHelper::DeviceExtensionsAreSupported(VkPhysicalDevice device)
{
    // get the count of all the extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    // get all the available extensions
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // get all the required extensions
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // check if the required extension exists, and if so remove it
    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    // were all the required extensions accounted for? or were there any that weren't removed?
    return requiredExtensions.empty();
}
