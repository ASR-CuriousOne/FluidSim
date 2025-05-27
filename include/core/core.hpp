#ifndef CORE_HPP
#define CORE_HPP
#include "renderer/vulkanContext.hpp"
#include "renderer/texture.hpp"
#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Core {
	
	class FluidSim{
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkSurfaceKHR m_surface;
		GLFWwindow* m_window;
		
		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::vector<VkBuffer> shaderStorageBuffers;
		std::vector<VkDeviceMemory> shaderStorageBuffersMemory;

		std::vector<Renderer::Texture> velocityPressureAndDye;

public:
		FluidSim(Renderer::VulkanContext Context);

		void createComputePipeline();

		void createShaderStorageBuffers();

		void createAllTextures();
	};
}

#endif
