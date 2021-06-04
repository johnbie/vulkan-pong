#pragma once
#include "VulkanEngine.h"
#include "VulkanEngineHelper.h"
#include "DebugHelper.h"

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
#include <set>
//#include <unordered_map>

VulkanEngine::VulkanEngine(GLFWwindow* window, bool initialize)
{
    // glfw window instance
    this->window = window; 

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
    if (DebugHelper::ENABLE_VALIDATION_LAYERS)
        DebugHelper::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); // delete debug utils

    vkDestroySurfaceKHR(instance, surface, nullptr); // delete surface instance, which is device-specific
    vkDestroyInstance(instance, nullptr); // delete instance
}

// create instance for vulkan, which is needed to do vulkan stuff
void VulkanEngine::createInstance()
{
    // if validation layer is on but not all the requested layers are available
    if (DebugHelper::ENABLE_VALIDATION_LAYERS && !VulkanEngineHelper::ValidationLayersAreSupported())
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
    auto extensions = VulkanEngineHelper::RequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (DebugHelper::ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanEngineHelper::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanEngineHelper::validationLayers.data();

        DebugHelper::PopulateDebugMessengerCreateInfo(debugCreateInfo);
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

// setup debugger instance
void VulkanEngine::setupDebugMessenger()
{
    if (!DebugHelper::ENABLE_VALIDATION_LAYERS) return;

    // setup code
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    DebugHelper::PopulateDebugMessengerCreateInfo(createInfo); // populate data

    // if failed
    if (DebugHelper::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug messenger!");
}

// for surface info, which is device-specific
void VulkanEngine::createSurface()
{
    // can take simple parameters instead of structs, which is amazing
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        throw std::runtime_error("failed to create surface for window!");
}

// for picking a gpu
void VulkanEngine::pickPhysicalDevice()
{
    // get number of gpu available
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // none found
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    // list devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // look for the first suitable gpu
    // if you want to find the best suitable gpu, it's available on the physical devices tutorial: https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Physical_devices_and_queue_families#page_Base-device-suitability-checks
    for (const auto& device : devices)
    {
        if (DeviceIsSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    // no suitable gpu found
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

// create logical device
void VulkanEngine::createLogicalDevice()
{
    // look for queues with graphic capabilities
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    // set queue priority; needed even if there's only one queue
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // set up device features config
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE; // request anisotropy sampler feature

    // create a logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    // device extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanEngineHelper::deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = VulkanEngineHelper::deviceExtensions.data();

    if (DebugHelper::ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanEngineHelper::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanEngineHelper::validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;

    // error check
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
        throw std::runtime_error("failed to create logical device!");

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentationQueue);
}

// for creating swap chains, which allow frame-by-frame rendering via image queuing
void VulkanEngine::createSwapChain()
{
    // check for swap chain support
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = VulkanEngineHelper::ChooseSwapSurfaceFormat(swapChainSupport.formats); // surface formats aka color depth
    VkPresentModeKHR presentMode = VulkanEngineHelper::ChooseSwapPresentMode(swapChainSupport.presentModes); // presentation mode aka the conditions
    VkExtent2D extent = VulkanEngineHelper::ChooseSwapExtent(window, swapChainSupport.capabilities); // extent aka resolution of images

    // minimum image count to go for in a swap chain
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    // maximum image count to avoid exceeding. if it's zero, there's no limit.
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // fill in swap chain creation info
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // one layer per image; doesn't change unless doing complex like a stereoscopic 3D application
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // get queue families for swap chaining
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    // are the two families different?
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // share
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // greed
        // createInfo.queueFamilyIndexCount = 0; // optional; explicit
        // createInfo.pQueueFamilyIndices = nullptr; // optional; explicit
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform; // don't do flips, 180s, or any of that crazy shit
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // no transparency needed
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE; // you can ignore pixels that the user can't see

    createInfo.oldSwapchain = VK_NULL_HANDLE; // more on this later, but is related to window resizing and other forms of swap chain invalidation

    // create the swap chain
    if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // set up images
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

    // set image format and extent aka resolution
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

// for creating image view, which specifies what part of the image should be accessed in what way
void VulkanEngine::createImageViews()
{
    // image view vector should be resized to match the swapchain image vector size
    swapChainImageViews.resize(swapChainImages.size());

    // iterate over swap chain images
    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1); // create image views
    }
}

void VulkanEngine::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

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


// checks if device is suitable for what we want to do
// this includes supporting swapchains
bool VulkanEngine::DeviceIsSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    // does the specific device support all the extensions we need?
    bool extensionsSupported = VulkanEngineHelper::DeviceExtensionsAreSupported(device);

    // does the device have adquate swap chain support? (supporting it isn't enough)
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    // are all the requested physical device features supported?
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    // yes/no
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

SwapChainSupportDetails VulkanEngine::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    // check for basic surface capacities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    // get the count of supported surface capacities
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    // nonzero supported formats
    if (formatCount != 0) {
        // update size and add the other data
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // get the count of supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    // nonzero supported formats
    if (presentModeCount != 0) {
        // update size and add the other data
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

// find queue families, which does something
QueueFamilyIndices VulkanEngine::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    // get list of queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // finds at least one queue family
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;

            // present support
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

// for creating an image view
VkImageView VulkanEngine::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
    // set up image view creation info
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        throw std::runtime_error("failed to create image view!");

    return imageView;
}

void VulkanEngine::cleanSwapchain() {} // clean swapchain
