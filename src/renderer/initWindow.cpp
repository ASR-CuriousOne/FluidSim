#include "renderer/renderer.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#define HEIGHT 800
#define WIDTH 800

namespace Renderer {
	void VulkanRender::InitializeWindow(){
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
	}

	void VulkanRender::createSurface(){
		if(glfwCreateWindowSurface(m_vkInstance, m_window, nullptr,&m_surface) != VK_SUCCESS){
			throw std::runtime_error("failed to create window surface!");
		}

	}

	
}
