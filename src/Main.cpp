#include <fstream>
#include <Utilities/LabAsset.hpp>
#include <Utilities/Random.hpp>
#include <Event/EventHandler.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <VEZ.h>

using namespace Labyrinth;

int main(int argc, char* argv[]) {

    VezApplicationInfo AppInfo = {};
    AppInfo.pApplicationName = "Labyrinth";
    AppInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    AppInfo.pEngineName = "Labyrinth";
    AppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

    VezInstanceCreateInfo InstanceCreateInfo = {};
    InstanceCreateInfo.pApplicationInfo = &AppInfo;

    std::vector<const char*> EnabledLayers = { "VK_LAYER_LUNARG_standard_validation" };

    InstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(EnabledLayers.size());
    InstanceCreateInfo.ppEnabledLayerNames = EnabledLayers.data();

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    Extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
    InstanceCreateInfo.ppEnabledExtensionNames = Extensions.data();

    VkInstance Instance = VK_NULL_HANDLE;
    VkResult Result = VK_SUCCESS;
    Result = vezCreateInstance(&InstanceCreateInfo, &Instance);
    if(Result != VK_SUCCESS) {
        errorln("Error making Vulkan Instance");
        return 1;
    }

    GLFWwindow* Window;
    if(!glfwInit()) {
        errorln("GLFW failed to init!");
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Window = glfwCreateWindow(1280, 720, "Labyrinth", nullptr, nullptr);
    if(!Window) {
        errorln("Failed to create GLFW window!");
        return 1;
    }

    VkSurfaceKHR Surface;
    Result = glfwCreateWindowSurface(Instance, Window, nullptr, &Surface);
    if(Result != VK_SUCCESS) {
        errorln("Failed to create vulkan surface!");
        return 1;
    }


    vezDestroyInstance(Instance);

    glfwTerminate();
    
    return 0;
}