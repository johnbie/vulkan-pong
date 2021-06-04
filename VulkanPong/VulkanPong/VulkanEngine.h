#pragma once
#include "AppObject.h"
#include "Vertex.h"
#include "pch.h"

#include <vector>

class VulkanEngine : public AppObject
{
public:
    VulkanEngine(GLFWwindow* window, bool initialize = false);
protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    // constants
    const int MAX_FRAMES_IN_FLIGHT = 2;

    GLFWwindow* window; // glfw window instance
    VkInstance instance; // the vulkan instance

    VkDebugUtilsMessengerEXT debugMessenger; // explicit debug messenger
    VkSurfaceKHR surface; // surface
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // physical device
    VkDevice logicalDevice; // logical device
    VkQueue graphicsQueue; // graphic queue handler
    VkQueue presentationQueue; // presentation queue handler

    VkSwapchainKHR swapChain; // swap chain handler
    std::vector<VkImage> swapChainImages; // images rendered for swap chaining
    VkFormat swapChainImageFormat; // image format
    VkExtent2D swapChainExtent; // extent aka resolution
    std::vector<VkImageView> swapChainImageViews; // a view into the image, which specifies what part of the image should be accessed in what way
    std::vector<VkFramebuffer> swapChainFramebuffers; // the swap chain frame buffer, which is what you need to actually render something on the screen

    VkRenderPass renderPass; // render pass, which contains various render information to apply
    VkDescriptorSetLayout descriptorSetLayout; // descriptor layout
    VkPipelineLayout pipelineLayout; // graphic pipeline layout, which is needed for graphic pipeline stuff
    VkPipeline graphicsPipeline; // graphic pipleine (the real thing)

    VkCommandPool commandPool; // drawing command
    std::vector<VkCommandBuffer> commandBuffers; // the drawing command buffer

    std::vector<Vertex> vertices; // non-global vertices
    VkBuffer vertexBuffer; // the vertex buffer
    VkDeviceMemory vertexBufferMemory; // memory heap for storing vertex information

    std::vector<uint32_t> indices; // non-global indices
    VkBuffer indexBuffer; // the index buffer
    VkDeviceMemory indexBufferMemory; // memory heap for storing index information

    std::vector<VkBuffer> uniformBuffers; // the uniform buffers
    std::vector<VkDeviceMemory> uniformBuffersMemory; // memory heap for storing uniform buffers

    VkDescriptorPool descriptorPool; // pool for descriptors
    std::vector<VkDescriptorSet> descriptorSets; // collection of descriptor sets

    std::vector<VkSemaphore> imageAvailableSemaphores; // image ready for rendering; not presentable yet
    std::vector<VkSemaphore> renderFinishedSemaphores; // image ready for presenting; rendered
    std::vector<VkFence> inFlightFences; // 
    std::vector<VkFence> imagesInFlight; // 
    size_t currentFrame = 0; // tracks the current frame of the application. 0 <= currentFrame < MAX_FRAMES_IN_FLIGHT

    void createInstance(); // create vulkan instance
    void setupDebugMessenger(); // setup debug messenger
    void createSurface(); // for surface info, which is device-specific
    void pickPhysicalDevice(); // for looking for graphics card in system
    void createLogicalDevice(); // for logical device, which is needed for figuring out what device should be used for our application
    void createSwapChain(); // for swap chains, which allow frame-by-frame rendering via image queuing
    void createImageViews(); // for creating an image view, which specifies what part of the image should be accessed in what way
    void createRenderPass(); // for creating a render pass, which contains various render information to apply
    void createDescriptorSetLayout(); // for creating the descriptor for the vertex shader
    void createGraphicsPipeline(); // for setting up graphics pipeline, which is turns a coordinate of positions & colors into a collection of colored pixels
    void createColorResources(); // for setting up color resources, which is needed for multisampling
    void createDepthResources(); // for setting up the depth resources, which is what renders the right image on top of the other
    void createFramebuffers(); // for setting up the swap chain frame buffer, which is what you need to actually render something on the screen
    void createCommandPool(); // for setting up the command pool, which is what sends the draw commands to vulkan

    void createVertexBuffer(); // for setting up the vertex buffers, which is what's needed to render vert
    void createIndexBuffer(); // for setting up the index buffers, which prevents vertex duplication inefficiency
    void createUniformBuffers(); // for setting up the uniform buffer, which is needed for sampling later
    void createDescriptorPool(); // for setting up the descriptor pool, which does stuff
    void createDescriptorSets(); // for setting up the descriptor sets, which does stuff
    void createCommandBuffers(); // for setting up the command buffer, which is the collection of commands
    void createSyncObjects(); // for setting up the various sync objects, which are needed for real-time drawing
    void cleanSwapchain(); // clean swapchain

    // helper functions
    bool checkValidationLayerSupport(); // used with createInstance()
};