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


        std::vector<const char*> GetRequiredExtensions();
        bool                     CheckForValidationLayerSupport();

        QueueFamilyIndices       FindDeviceQueueFamilies(VkPhysicalDevice Device);

        bool                     CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice Device);
        bool                     IsPhysicalDeviceSuitable(VkPhysicalDevice Device);



        int                      CreateInstance();
        int                      CreateDebugCallbacks();



        void                     Cleanup();

    public:

         Renderer();
        ~Renderer();


        Window& GetWindow() { return mWindow; }


    };

    Renderer::Renderer() : IRenderer(GAPI::Vulkan) {
        CreateInstance();
        mWindow.Open();
        CreateDebugCallbacks();
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
        /*QueueFamilyIndices indices = findQueueFamilies(device);

        bool extensionsSupported = CheckPhysicalDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;*/
        return false;
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


    void Renderer::Cleanup() {
        if(mDebugCallback != VK_NULL_HANDLE) { DestroyDebugUtilsMessengerEXT(mInstance, mDebugCallback, nullptr); }
        if(mInstance      != VK_NULL_HANDLE) { vezDestroyInstance(mInstance); }
        mWindow.Terminate();
        glfwTerminate();
    }

}