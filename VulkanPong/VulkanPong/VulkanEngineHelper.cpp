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
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // extension only needed in debug mode

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
        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        // requested layer not found
        if (!layerFound)
            return false;
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


// chooses the best available surface format to do swap chains with
// checks based on surface format (aka color depth), presentation mode (the conditions), and swap extent (resolution of images)
VkSurfaceFormatKHR VulkanEngineHelper::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    // checks for format that supports what is most ideal
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }

    // returns the first available format
    return availableFormats[0];
}

// choose the presentation mode aka the actual conditions of showing an image
VkPresentModeKHR VulkanEngineHelper::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    // triple-buffering mode if available; avoids tearing
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
            return availablePresentMode;
    }

    // queue (aka first-in-first-out) mode; avoids tearing
    return VK_PRESENT_MODE_FIFO_KHR;
}

// choose the actual extent aka the actual resolution
VkExtent2D VulkanEngineHelper::ChooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities)
{
    // if there's already a resolution set...
    if (capabilities.currentExtent.width != UINT32_MAX) 
        return capabilities.currentExtent; // return it
    else 
    {
        // get size based on glfw's discretion
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = 
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        // max of two minimums
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}