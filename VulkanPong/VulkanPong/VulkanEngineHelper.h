#pragma once
#include "pch.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

#include <vector>
#include <string>

class VulkanEngineHelper
{
public:
    static const std::vector<const char*> validationLayers;
    static const std::vector<const char*> deviceExtensions;

    static std::vector<const char*> RequiredExtensions();
    static bool ValidationLayersAreSupported();
    static bool DeviceExtensionsAreSupported(VkPhysicalDevice device);

    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D ChooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);

    static std::vector<char> ReadFile(const std::string& filename);
};