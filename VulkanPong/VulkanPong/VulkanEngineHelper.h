#pragma once
#include "pch.h"

#include <vector>

class VulkanEngineHelper
{
public:
    static const std::vector<const char*> validationLayers;
    static const std::vector<const char*> deviceExtensions;
    static std::vector<const char*> getRequiredExtensions();
    static bool checkValidationLayerSupport();

};