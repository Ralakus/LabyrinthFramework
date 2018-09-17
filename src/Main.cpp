#if 1
#include <fstream>
#include <Utilities/LabAsset.hpp>
#include <Utilities/Random.hpp>
#include <Event/EventHandler.hpp>

#include <Graphics/Vulkan/Renderer.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <VEZ.h>
#include <glm/glm.hpp>

#include <stb_image.h>

using namespace Labyrinth;

typedef struct PipelineDesc
{
    VezPipeline Pipeline = VK_NULL_HANDLE;
    std::vector<VkShaderModule> ShaderModules;
} PipelineDesc;

struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct UniformBuffer
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

int main(int argc, char* argv[]) {



    GStreams::Buffer(true, false);

    lvk::Renderer Renderer;
    lvk::Window*  Window = static_cast<lvk::Window*>(Renderer.GetWindow());

    double LastTime = glfwGetTime();
    double Delta    = 0.0;
    double OnSecondTimer = 0.0;
    double AvgDelta = 0.0;
    size_t Samples  = 0.0;




    /*VkQueue GraphicsQueue = VK_NULL_HANDLE;
    VkBuffer VertexBuffer = VK_NULL_HANDLE;
    VkBuffer IndexBuffer = VK_NULL_HANDLE;
    VkImage Image = VK_NULL_HANDLE;
    VkImageView ImageView = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;
    VkBuffer UniformBuffer = VK_NULL_HANDLE;
    PipelineDesc BasicPipeline;
    VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;

    Vertex vertices[] = {
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
    };

    // Create the device side vertex buffer.
    VezBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.size = sizeof(vertices);
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;    
    auto result = vezCreateBuffer(Renderer.mDevice, VEZ_MEMORY_GPU_ONLY, &bufferCreateInfo, &VertexBuffer);
    if (result != VK_SUCCESS){
        errorln("Failed to create vertex buffer!");
        return EXIT_FAILURE;
    }
        

    // Upload the host side data.
    result = vezBufferSubData(Renderer.mDevice, VertexBuffer, 0, sizeof(vertices), static_cast<void*>(vertices));
    if (result != VK_SUCCESS) {
        errorln("vezBufferSubData failed for vertex buffer");
        return EXIT_FAILURE;
    }

    // A single quad with positions, normals and uvs.
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    // Create the device side index buffer.
    bufferCreateInfo.size = sizeof(indices);
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    result = vezCreateBuffer(Renderer.mDevice, VEZ_MEMORY_GPU_ONLY, &bufferCreateInfo, &IndexBuffer);
    if (result != VK_SUCCESS) {
        errorln("vezCreateBuffer failed for index buffer");
        return EXIT_FAILURE;
    }

    // Upload the host side data.
    result = vezBufferSubData(Renderer.mDevice, IndexBuffer, 0, sizeof(indices), static_cast<void*>(indices));
    if (result != VK_SUCCESS) {
        errorln("vezBufferSubData failed for index buffer"); 
        return EXIT_FAILURE;
    }


    int width, height, channels;
    auto pixelData = stbi_load("../../Samples/Data/Textures/texture.jpg", &width, &height, &channels, 4);

    // Create the Renderer.mDevice side image.
    VezImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageCreateInfo.extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    auto result = vezCreateImage(Renderer.mDevice, VEZ_MEMORY_GPU_ONLY, &imageCreateInfo, &Image);
    if (result != VK_SUCCESS)
        errorln("vezCreateImage failed");

    // Upload the host side data.
    VezImageSubDataInfo subDataInfo = {};
    subDataInfo.imageSubresource.mipLevel = 0;
    subDataInfo.imageSubresource.baseArrayLayer = 0;
    subDataInfo.imageSubresource.layerCount = 1;
    subDataInfo.imageOffset = { 0, 0, 0 };
    subDataInfo.imageExtent = { imageCreateInfo.extent.width, imageCreateInfo.extent.height, 1 };
    result = vezImageSubData(Renderer.mDevice, Image, &subDataInfo, reinterpret_cast<const void*>(pixelData));
    if (result != VK_SUCCESS) {
        errorln("vezImageSubData failed");
        return EXIT_FAILURE;
    }

    // Destroy the pixel data.
    stbi_image_free(pixelData);

    // Create the image view for binding the texture as a resource.
    VezImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.image = Image;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = imageCreateInfo.format;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    result = vezCreateImageView(Renderer.mDevice, &imageViewCreateInfo, &ImageView);
    if (result != VK_SUCCESS) {
        errorln("vezCreateImageView failed");
        return EXIT_FAILURE;
    }*/


    while(Window->IsValid()) {
        Delta = glfwGetTime() - LastTime;
        LastTime += Delta;
        OnSecondTimer += Delta;
        Samples++;

        if(OnSecondTimer >= 1.0) {
            AvgDelta = (OnSecondTimer / static_cast<double>(Samples));
            OnSecondTimer = 0.0;
            Samples = 0;
            noticeln("FPS: ", std::to_string(1.0 / AvgDelta));
        }

        Window->Update();



    }
    
    return EXIT_SUCCESS;
}

#endif