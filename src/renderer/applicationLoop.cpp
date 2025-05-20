#include "renderer/renderer.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Renderer {
	void VulkanRender::StartLoop(void(*Update)()){
		while(!glfwWindowShouldClose(m_window)){
			glfwPollEvents();

			Update();
			drawFrame();
			
		}

		vkDeviceWaitIdle(m_device);

		std::cout << "Application Closed" << std::endl;

	}
}
