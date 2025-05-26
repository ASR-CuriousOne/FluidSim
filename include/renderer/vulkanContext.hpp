#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Renderer {
	struct VulkanContext{
		VkInstance m_vkInstance = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device = VK_NULL_HANDLE;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		GLFWwindow* m_window = nullptr;
	};

}
#endif
