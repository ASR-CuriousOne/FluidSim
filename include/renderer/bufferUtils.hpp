#ifndef BUFFER_UTILS_HPP
#define BUFFER_UTILS_HPP
#include <cstdint>
#include <vulkan/vulkan.h>
namespace Renderer {
	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createBuffer(VkDevice device,VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void copyBuffer(VkDevice device,VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,VkQueue graphicsQueue,VkCommandPool commandPool);

}
#endif
