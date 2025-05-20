#ifndef IMAGE_UTILS_HPP
#define IMAGE_UTILS_HPP
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Renderer {
    VkImageView createImageViewForImage(VkDevice device, VkImage image, VkFormat format,VkImageAspectFlags aspectFlag);

    void copyBufferToImage(VkBuffer buffer, VkImage image, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, uint32_t width, uint32_t height);

    void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
            VkImage& image, VkDeviceMemory& imageMemory);

	VkFormat findDepthFormat(VkPhysicalDevice physcialDevice);

	bool hasStencilComponent(VkFormat format);
}
#endif
