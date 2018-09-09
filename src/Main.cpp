#if 1
#include <fstream>
#include <Utilities/LabAsset.hpp>
#include <Utilities/Random.hpp>
#include <Event/EventHandler.hpp>

#include <Graphics/Vulkan/Renderer.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <VEZ.h>

using namespace Labyrinth;

std::vector<const char*> ValidationLayers = { "VK_LAYER_LUNARG_standard_validation" };

int main(int argc, char* argv[]) {

    /*if(!CheckValidationLayerSupport()) {
        errorln("No vulkan validation layer support!");
        return EXIT_FAILURE;
    }

    if(!glfwInit()) {
        errorln("GLFW failed to init!");
        return EXIT_FAILURE;
    }

    VezApplicationInfo AppInfo = {};
    AppInfo.pApplicationName = "Labyrinth";
    AppInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    AppInfo.pEngineName = "Labyrinth";
    AppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

    VezInstanceCreateInfo InstanceCreateInfo = {};
    InstanceCreateInfo.pApplicationInfo = &AppInfo;


    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
    InstanceCreateInfo.ppEnabledExtensionNames = Extensions.data();
    
    InstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
    InstanceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();

    VkInstance Instance = VK_NULL_HANDLE;
    VkResult Result = VK_SUCCESS;
    Result = vezCreateInstance(&InstanceCreateInfo, &Instance);
    if(Result != VK_SUCCESS) {
        errorln("Error making vulkan instance");
        return EXIT_FAILURE;
    }

    VkDebugUtilsMessengerEXT Callback;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;

    Result = CreateDebugUtilsMessengerEXT(Instance, &createInfo, nullptr, &Callback);
    if(Result != VK_SUCCESS) {
        errorln("Failed to create debug callbacks!");
        return EXIT_FAILURE;
    }

    uint32_t PhysicalDeviceCount = 0;
    vezEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, nullptr);
    if (PhysicalDeviceCount == 0)
    {
        errorln("No vulkan physical devices found");
        return EXIT_FAILURE;
    }

    std::vector<VkPhysicalDevice> PhysicalDevices(PhysicalDeviceCount);
    vezEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, PhysicalDevices.data());

    for (auto pd : PhysicalDevices)
    {
        VkPhysicalDeviceProperties properties = {};
        vezGetPhysicalDeviceProperties(pd, &properties);
        noticeln(properties.deviceName);
    }

    VkPhysicalDevice PhysicalDevice = PhysicalDevices[0];

    VkPhysicalDeviceProperties properties = {};
    vezGetPhysicalDeviceProperties(PhysicalDevice, &properties);
    noticeln("Selected device: ", properties.deviceName);

    GLFWwindow* Window;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Window = glfwCreateWindow(1280, 720, "Labyrinth", nullptr, nullptr);
    if(!Window) {
        errorln("Failed to create GLFW window!");
        return EXIT_FAILURE;
    }

    VkSurfaceKHR Surface;
    Result = glfwCreateWindowSurface(Instance, Window, nullptr, &Surface);
    if(Result != VK_SUCCESS) {
        errorln("Failed to create vulkan surface!");
        return EXIT_FAILURE;
    }


    std::vector<const char*> EnabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VezDeviceCreateInfo DeviceCreateInfo = {};
    DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(EnabledExtensions.size());
    DeviceCreateInfo.ppEnabledExtensionNames = EnabledExtensions.data();

    VkDevice Device = VK_NULL_HANDLE;
    Result = vezCreateDevice(PhysicalDevice, &DeviceCreateInfo, &Device);
    if(Result != VK_SUCCESS) {
        errorln("Failed to create vulkan device!");
        return EXIT_FAILURE;
    }

    VezSwapchainCreateInfo SwapChainCreateInfo = {};
    SwapChainCreateInfo.surface = Surface;
    SwapChainCreateInfo.format = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

    VezSwapchain SwapChain = VK_NULL_HANDLE;
    Result = vezCreateSwapchain(Device, &SwapChainCreateInfo, &SwapChain);
    if(Result != VK_SUCCESS) {
        errorln("Failed to create vulkan swapchain!");
        return EXIT_FAILURE;
    }



    while(!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
    }




    DestroyDebugUtilsMessengerEXT(Instance, Callback, nullptr);

    vkDestroySurfaceKHR(Instance, Surface, nullptr);

    vezDestroyDevice(Device);
    vezDestroyInstance(Instance);

    glfwDestroyWindow(Window);
    glfwTerminate();*/

    lvk::Renderer Renderer;
    lvk::Window& Window = Renderer.GetWindow();

    while(Window.IsValid()) {
        Window.Update();
    }
    
    return EXIT_SUCCESS;
}

#endif