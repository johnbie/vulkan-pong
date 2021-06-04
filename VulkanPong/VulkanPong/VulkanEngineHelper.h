#pragma once
#include "pch.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

#include <vector>

class VulkanEngineHelper
{
public:
    static const std::vector<const char*> validationLayers;
    static const std::vector<const char*> deviceExtensions;

    static std::vector<const char*> RequiredExtensions();
    static bool ValidationLayersAreSupported();
    static bool DeviceExtensionsAreSupported(VkPhysicalDevice device);
};