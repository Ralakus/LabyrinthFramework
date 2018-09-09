#pragma once
#include <vector>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <VEZ.h>

#include "../IRenderer.hpp"
#include "Window.hpp"
#include <Utilities/IO.hpp>


namespace Labyrinth::lvk {

    static inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }

    static inline VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        errorln("[ vk:error ]: ", pCallbackData->pMessage);

        return VK_FALSE;
    }

    struct QueueFamilyIndices {
        int GraphicsFamily = -1;
        int PresentFamily = -1;

        bool IsComplete() {
            return GraphicsFamily >= 0 && PresentFamily >= 0;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    class Renderer : public IRenderer {

        static const std::vector<const char*>& ValidationLayers() {
            static std::vector<const char*> mLayers = { "VK_LAYER_LUNARG_standard_validation" };
            return mLayers;
        }

        static const std::vector<const char*>& DeviceExtensions() {
            static std::vector<const char*> mExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            return mExtensions;
        }

        bool                        mEnableValidationLayers  = true;
        VkDebugUtilsMessengerEXT    mDebugCallback           = VK_NULL_HANDLE;

        VkInstance                  mInstance                = VK_NULL_HANDLE;

        VkPhysicalDevice            mPhysicalDevice          = VK_NULL_HANDLE;
        VkDevice                    mDevice                  = VK_NULL_HANDLE;

        Window                      mWindow                  = {};
        bool                        mHasSurface              = false;
        VkSurfaceKHR                mSurface                 = VK_NULL_HANDLE;
        VezSwapchain                mSwapChain               = VK_NULL_HANDLE;

        struct {
            VkImage        ColorImage            = VK_NULL_HANDLE;
            VkImageView    ColorImageView        = VK_NULL_HANDLE;
            VkImage        DepthStencilImage     = VK_NULL_HANDLE;
            VkImageView    DepthStencilImageView = VK_NULL_HANDLE;
            VezFramebuffer Handle                = VK_NULL_HANDLE;
        }                  mFrameBuffer;


        std::vector<const char*> GetRequiredExtensions();
        bool                     CheckForValidationLayerSupport();

        int                      CreateInstance();
        int                      CreateDebugCallbacks();



        QueueFamilyIndices       FindDeviceQueueFamilies(VkPhysicalDevice Device);
        SwapChainSupportDetails  QuerySwapChainSupportDetails(VkPhysicalDevice Device);
        bool                     CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice Device);
        bool                     IsPhysicalDeviceSuitable(VkPhysicalDevice Device);



        int                      PickPhysicalDevice();

        int                      CreateLogicalDevice();

        int                      CreateSwapChain();

        int                      CreateFrameBuffer();
        int                      CleanupFrameBuffer();

        void                     Cleanup();

    public:

         Renderer();
        ~Renderer();


        virtual void     Update()    override;
        virtual IWindow* GetWindow() override { return &mWindow; }



    };




    Renderer::Renderer() : IRenderer(GAPI::Vulkan) {
        CreateInstance();
        mWindow.Open();
        mWindow.mRendererParent = this;
        VkResult Result = glfwCreateWindowSurface(mInstance, mWindow.mWindow, nullptr, &mSurface);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan surface!");
        }
        mHasSurface = true;
        CreateDebugCallbacks();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateFrameBuffer();
    }
    Renderer::~Renderer() {
        Cleanup();
    }






    std::vector<const char*> Renderer::GetRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (mEnableValidationLayers) {
            Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return Extensions;
    }

    bool Renderer::CheckForValidationLayerSupport() {
        uint32_t LayerCount;
        vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

        std::vector<VkLayerProperties> AvailableLayers(LayerCount);
        vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

        std::set<std::string> RequiredLayers(ValidationLayers().begin(), ValidationLayers().end());

        for(auto& Layer : AvailableLayers) {
            RequiredLayers.erase(Layer.layerName);
        }

        return RequiredLayers.empty();
    }


    int Renderer::CreateInstance() {

        if(!glfwInit()) {
            errorln("Failed it init glfw!");
            return -1;
        }

        if (mEnableValidationLayers && !CheckForValidationLayerSupport()) {
            warnln("Vulkan validation layers requested, but not available!");
            mEnableValidationLayers = false;
        }

        VezApplicationInfo AppInfo = {};
        AppInfo.pApplicationName = "Labyrinth";
        AppInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        AppInfo.pEngineName = "Labyrinth";
        AppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

        VezInstanceCreateInfo InstanceCreateInfo = {};
        InstanceCreateInfo.pApplicationInfo = &AppInfo;

        auto Extensions = GetRequiredExtensions();
        InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
        InstanceCreateInfo.ppEnabledExtensionNames = Extensions.data();

        if (mEnableValidationLayers) {
            InstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers().size());
            InstanceCreateInfo.ppEnabledLayerNames = ValidationLayers().data();
        } else {
            InstanceCreateInfo.enabledLayerCount = 0;
        }

        VkResult Result = vezCreateInstance(&InstanceCreateInfo, &mInstance);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan instance!");
            return -1;
        }

        return 0;
    }

    int Renderer::CreateDebugCallbacks() {

        VkDebugUtilsMessengerCreateInfoEXT DebugCallbackCreateInfo = {};
        DebugCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        DebugCallbackCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        DebugCallbackCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        DebugCallbackCreateInfo.pfnUserCallback = DebugCallback;

        VkResult Result = CreateDebugUtilsMessengerEXT(mInstance, &DebugCallbackCreateInfo, nullptr, &mDebugCallback);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan debug callbacks!");
            return -1;
        }

        return 0;

    }








     QueueFamilyIndices Renderer::FindDeviceQueueFamilies(VkPhysicalDevice Device) {
        QueueFamilyIndices Indices;

        uint32_t QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

        int i = 0;
        for (const auto& QueueFamily : QueueFamilies) {
            if (QueueFamily.queueCount > 0 && QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                Indices.GraphicsFamily = i;
            }

            VkBool32 PresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, mSurface, &PresentSupport);

            if (QueueFamily.queueCount > 0 && PresentSupport) {
                Indices.PresentFamily = i;
            }

            if (Indices.IsComplete()) {
                break;
            }

            i++;
        }

        return Indices;
    }

    SwapChainSupportDetails Renderer::QuerySwapChainSupportDetails(VkPhysicalDevice Device) {
        SwapChainSupportDetails Details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, mSurface, &Details.Capabilities);

        uint32_t FormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, mSurface, &FormatCount, nullptr);

        if (FormatCount != 0) {
            Details.Formats.resize(FormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(Device, mSurface, &FormatCount, Details.Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, mSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            Details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(Device, mSurface, &presentModeCount, Details.PresentModes.data());
        }

        return Details;
    }

    bool Renderer::CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice Device) {
        uint32_t ExtensionCount;
        vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);

        std::vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
        vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());

        std::set<std::string> RequiredExtensions(DeviceExtensions().begin(), DeviceExtensions().end());

        for (const auto& Extension : AvailableExtensions) {
            RequiredExtensions.erase(Extension.extensionName);
        }

        return RequiredExtensions.empty();
    }

    bool Renderer::IsPhysicalDeviceSuitable(VkPhysicalDevice Device) {
        QueueFamilyIndices Indices = FindDeviceQueueFamilies(Device);

        bool ExtensionsSupported = CheckPhysicalDeviceExtensionSupport(Device);

        bool SwapChainAdequate = false;
        if (ExtensionsSupported) {
            SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupportDetails(Device);
            SwapChainAdequate = !SwapChainSupport.Formats.empty() && !SwapChainSupport.PresentModes.empty();
        }

        return Indices.IsComplete() && ExtensionsSupported && SwapChainAdequate;
    }


    int Renderer::PickPhysicalDevice() {
        uint32_t DeviceCount = 0;
        vkEnumeratePhysicalDevices(mInstance, &DeviceCount, nullptr);

        if (DeviceCount == 0) {
            errorln("Failed to find any physical device with vulkan support!");
            return -1;
        }

        std::vector<VkPhysicalDevice> Devices(DeviceCount);
        vkEnumeratePhysicalDevices(mInstance, &DeviceCount, Devices.data());

        for (const auto& Device : Devices) {
            if (IsPhysicalDeviceSuitable(Device)) {
                mPhysicalDevice = Device;
                VkPhysicalDeviceProperties DeviceProperties;
                vezGetPhysicalDeviceProperties(Device, &DeviceProperties);
                noticeln("Using: \"", DeviceProperties.deviceName, "\"");
                break;
            }
        }

        if (mPhysicalDevice == VK_NULL_HANDLE) {
            errorln("Failed to find suitable physical device!");
            return -1;
        }

        return 0;
    }






    int Renderer::CreateLogicalDevice() {

        VezDeviceCreateInfo DeviceCreateInfo = {};
        DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions().size());
        DeviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions().data();

        VkResult Result = vezCreateDevice(mPhysicalDevice, &DeviceCreateInfo, &mDevice);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan logical device!");
            return -1;
        }

        return 0;
    }

    int Renderer::CreateSwapChain() {

        VezSwapchainCreateInfo SwapChainCreateInfo = {};
        SwapChainCreateInfo.surface = mSurface;
        SwapChainCreateInfo.format = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        SwapChainCreateInfo.tripleBuffer = VK_TRUE;
        
        VkResult Result = vezCreateSwapchain(mDevice, &SwapChainCreateInfo, &mSwapChain);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan swapchain!");
            return -1;
        }

        return 0;

    }



    int Renderer::CreateFrameBuffer() {

        if(mFrameBuffer.Handle != VK_NULL_HANDLE) {
            CleanupFrameBuffer();
        }

        VkSurfaceFormatKHR SwapChainFormat = {};
        vezGetSwapchainSurfaceFormat(mSwapChain, &SwapChainFormat);

        VezImageCreateInfo ImageCreateInfo = {};
        ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        ImageCreateInfo.format = SwapChainFormat.format;
        ImageCreateInfo.extent = { mWindow.mWidth, mWindow.mHeight, 1 };
        ImageCreateInfo.mipLevels = 1;
        ImageCreateInfo.arrayLayers = 1;
        ImageCreateInfo.samples = VK_SAMPLE_COUNT_16_BIT;
        ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        ImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        VkResult Result = vezCreateImage(mDevice, VEZ_MEMORY_GPU_ONLY, &ImageCreateInfo, &mFrameBuffer.ColorImage);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan color buffer image!");
            return -1;
        }

        VezImageViewCreateInfo ImageViewCreateInfo = {};
        ImageViewCreateInfo.image = mFrameBuffer.ColorImage;
        ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ImageViewCreateInfo.format = ImageCreateInfo.format;
        ImageViewCreateInfo.subresourceRange.layerCount = 1;
        ImageViewCreateInfo.subresourceRange.levelCount = 1;
        
        Result = vezCreateImageView(mDevice, &ImageViewCreateInfo, &mFrameBuffer.ColorImageView);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan color buffer image view!");
            return -1;
        }

        ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        ImageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
        ImageCreateInfo.extent = { mWindow.mWidth, mWindow.mHeight, 1 };
        ImageCreateInfo.mipLevels = 1;
        ImageCreateInfo.arrayLayers = 1;
        ImageCreateInfo.samples = VK_SAMPLE_COUNT_16_BIT;
        ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        ImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        Result = vezCreateImage(mDevice, VEZ_MEMORY_GPU_ONLY, &ImageCreateInfo, &mFrameBuffer.DepthStencilImage);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan depth buffer image!");
            return -1;
        }


        ImageViewCreateInfo.image = mFrameBuffer.ColorImage;
        ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ImageViewCreateInfo.format = ImageCreateInfo.format;
        ImageViewCreateInfo.subresourceRange.layerCount = 1;
        ImageViewCreateInfo.subresourceRange.levelCount = 1;

        Result = vezCreateImageView(mDevice, &ImageViewCreateInfo, &mFrameBuffer.DepthStencilImageView);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan depth buffer image view!");
            return -1;
        }

        std::array<VkImageView, 2> Attachments = { mFrameBuffer.ColorImageView, mFrameBuffer.DepthStencilImageView };

        VezFramebufferCreateInfo FrameBufferCreateInfo = {};
        FrameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(Attachments.size());
        FrameBufferCreateInfo.pAttachments = Attachments.data();
        FrameBufferCreateInfo.width  = mWindow.mWidth;
        FrameBufferCreateInfo.height = mWindow.mHeight;
        FrameBufferCreateInfo.layers = 1;

        Result = vezCreateFramebuffer(mDevice, &FrameBufferCreateInfo, &mFrameBuffer.Handle);
        if(Result != VK_SUCCESS) {
            errorln("Failed to create vulkan framebuffer!");
            return -1;
        }
        

        return 0;
        
    }

    int Renderer::CleanupFrameBuffer() {
        vezDestroyFramebuffer(mDevice, mFrameBuffer.Handle);
        vezDestroyImageView  (mDevice, mFrameBuffer.ColorImageView);
        vezDestroyImageView  (mDevice, mFrameBuffer.DepthStencilImageView);
        vezDestroyImage      (mDevice, mFrameBuffer.ColorImage);
        vezDestroyImage      (mDevice, mFrameBuffer.DepthStencilImage);
        return 0;
    }



    void Renderer::Cleanup() {
        vezDeviceWaitIdle(mDevice);
        if(mSwapChain     != VK_NULL_HANDLE) { vezDestroySwapchain(mDevice, mSwapChain); }
        if(mDevice        != VK_NULL_HANDLE) { vezDestroyDevice(mDevice); }
        if(mDebugCallback != VK_NULL_HANDLE) { DestroyDebugUtilsMessengerEXT(mInstance, mDebugCallback, nullptr); }
        if(mSurface       != VK_NULL_HANDLE) { vkDestroySurfaceKHR(mInstance, mSurface, nullptr); }
        if(mInstance      != VK_NULL_HANDLE) { vezDestroyInstance(mInstance); }
        mWindow.Terminate();
        glfwTerminate();
    }




    void Renderer::Update() {



    }

}