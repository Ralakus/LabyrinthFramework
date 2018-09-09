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

    /*
    
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
    lvk::Window&  Window = Renderer.GetWindow();

    while(Window.IsValid()) {
        Window.Update();
    }
    
    return EXIT_SUCCESS;
}

#endif