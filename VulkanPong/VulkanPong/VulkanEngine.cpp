#pragma once
#include "VulkanEngine.h"
#include "VulkanEngineHelper.h"
#include "Debug.h"

// c++ libraries
#include <iostream>
//#include <fstream>
//#include <stdexcept>
//#include <algorithm>
//#include <chrono>
//#include <vector>
//#include <cstring>
//#include <cstdlib>
//#include <cstdint>
//#include <array>
//#include <optional>
//#include <set>
//#include <unordered_map>

VulkanEngine::VulkanEngine(bool initialize)
{
    if (initialize)
        this->initialize();
    else
    {
        instance = nullptr;
        debugMessenger = nullptr;
        surface = nullptr;
        physicalDevice = nullptr;
        logicalDevice = nullptr;
        graphicsQueue = nullptr;
        presentationQueue = nullptr;
        swapChain = nullptr;
        renderPass = nullptr;
        descriptorSetLayout = nullptr;
        pipelineLayout = nullptr;
        graphicsPipeline = nullptr;
        commandPool = nullptr;

        vertexBuffer = nullptr;
        vertexBufferMemory = nullptr;
        indexBuffer = nullptr;
        indexBufferMemory = nullptr;

        descriptorPool = nullptr;
        swapChainExtent = VkExtent2D{};
        swapChainImageFormat = VkFormat{};
    }
}
void VulkanEngine::initialize()
{
    createInstance(); // create vulkan instance
    setupDebugMessenger(); // setup debug messenger
    createSurface(); // for surface info, which is device-specific
    pickPhysicalDevice(); // for looking for graphics card in system
    createLogicalDevice(); // for logical device, which is needed for figuring out what device should be used for our application
    createSwapChain(); // for swap chains, which allow frame-by-frame rendering via image queuing
    createImageViews(); // for creating an image view, which specifies what part of the image should be accessed in what way
    createRenderPass(); // for creating a render pass, which contains various render information to apply
    createDescriptorSetLayout(); // for creating the descriptor for the vertex shader
    createGraphicsPipeline(); // for setting up graphics pipeline, which is turns a coordinate of positions & colors into a collection of colored pixels
    createColorResources(); // for setting up color resources, which is needed for multisampling
    createDepthResources(); // for setting up the depth resources, which is what renders the right image on top of the other
    createFramebuffers(); // for setting up the swap chain frame buffer, which is what you need to actually render something on the screen
    createCommandPool(); // for setting up the command pool, which is what sends the draw commands to vulkan
    createVertexBuffer(); // for setting up the vertex buffers, which is what's needed to render vert
    createIndexBuffer(); // for setting up the index buffers, which prevents vertex duplication inefficiency
    createUniformBuffers(); // for setting up the uniform buffer, which is needed for sampling later
    createDescriptorPool(); // for setting up the descriptor pool, which does stuff
    createDescriptorSets(); // for setting up the descriptor sets, which does stuff
    createCommandBuffers(); // for setting up the command buffer, which is the collection of commands
    createSyncObjects(); // for setting up the various sync objects, which are needed for real-time drawing
}

void VulkanEngine::update()
{
	;
}

void VulkanEngine::clean()
{
    // wait for processes to close
    // ignore the warning; we don't need to view the result anyways
	vkDeviceWaitIdle(logicalDevice); 
    
    cleanSwapchain();
    vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr); // delete descriptor

    vkDestroyBuffer(logicalDevice, indexBuffer, nullptr); // delete index buffer
    vkFreeMemory(logicalDevice, indexBufferMemory, nullptr); // free the memory reserved for index buffer

    vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr); // delete vertex buffer
    vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr); // free the memory reserved for vertex buffer

    // delete the sync objects
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(logicalDevice, commandPool, nullptr); // delete command pool
    vkDestroyDevice(logicalDevice, nullptr); // delete logical device

    // if validation layer was on...
    if (Debug::ENABLE_VALIDATION_LAYERS)
        Debug::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); // delete debug utils

    vkDestroySurfaceKHR(instance, surface, nullptr); // delete surface instance, which is device-specific
    vkDestroyInstance(instance, nullptr); // delete instance
}

// create instance for vulkan, which is needed to do vulkan stuff
void VulkanEngine::createInstance()
{
    // if validation layer is on but not all the requested layers are available
    if (Debug::ENABLE_VALIDATION_LAYERS && !VulkanEngineHelper::checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    // set up vulkan app info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // need to specify explicitly by design
    appInfo.pApplicationName = "Da Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // info about vulcan instance creation process
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // need to specify explicitly by design
    createInfo.pApplicationInfo = &appInfo;

    // get required extensions
    // extensions are the tools available for use on the device
    auto extensions = VulkanEngineHelper::getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (Debug::ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanEngineHelper::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanEngineHelper::validationLayers.data();

        Debug::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // error check
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}
void VulkanEngine::setupDebugMessenger() {} // setup debug messenger
void VulkanEngine::createSurface() {} // for surface info, which is device-specific
void VulkanEngine::pickPhysicalDevice() {} // for looking for graphics card in system
void VulkanEngine::createLogicalDevice() {} // for logical device, which is needed for figuring out what device should be used for our application
void VulkanEngine::createSwapChain() {} // for swap chains, which allow frame-by-frame rendering via image queuing
void VulkanEngine::createImageViews() {} // for creating an image view, which specifies what part of the image should be accessed in what way
void VulkanEngine::createRenderPass() {} // for creating a render pass, which contains various render information to apply
void VulkanEngine::createDescriptorSetLayout() {} // for creating the descriptor for the vertex shader
void VulkanEngine::createGraphicsPipeline() {} // for setting up graphics pipeline, which is turns a coordinate of positions & colors into a collection of colored pixels
void VulkanEngine::createColorResources() {} // for setting up color resources, which is needed for multisampling
void VulkanEngine::createDepthResources() {} // for setting up the depth resources, which is what renders the right image on top of the other
void VulkanEngine::createFramebuffers() {} // for setting up the swap chain frame buffer, which is what you need to actually render something on the screen
void VulkanEngine::createCommandPool() {} // for setting up the command pool, which is what sends the draw commands to vulkan

void VulkanEngine::createVertexBuffer() {} // for setting up the vertex buffers, which is what's needed to render vert
void VulkanEngine::createIndexBuffer() {} // for setting up the index buffers, which prevents vertex duplication inefficiency
void VulkanEngine::createUniformBuffers() {} // for setting up the uniform buffer, which is needed for sampling later
void VulkanEngine::createDescriptorPool() {} // for setting up the descriptor pool, which does stuff
void VulkanEngine::createDescriptorSets() {} // for setting up the descriptor sets, which does stuff
void VulkanEngine::createCommandBuffers() {} // for setting up the command buffer, which is the collection of commands
void VulkanEngine::createSyncObjects() {} // for setting up the various sync objects, which are needed for real-time drawing
void VulkanEngine::cleanSwapchain() {} // clean swapchain