#ifndef DEVICE_CREATION_HPP
#define DEVICE_CREATION_HPP
#include <vulkan/vulkan.h>
#include <renderer/queueFamiliesIndices.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Renderer {
    bool isDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR surface,const std::vector<const char*> deviceExtensions);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);

	

}

#endif
