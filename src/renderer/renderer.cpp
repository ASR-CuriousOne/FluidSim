#include "renderer/renderer.hpp"
#include "renderer/validationLayers.hpp"

#include <GLFW/glfw3.h>
#include <iostream>


namespace Renderer {	

	VulkanRender::VulkanRender(){
        std::cout << "Initializing Application......." << std::endl;
        InitializeWindow();
        InitializeVulkan();
        std::cout << "Application Initialized" << std::endl;

		

    }

	VulkanRender::~VulkanRender(){

		std::cout << "Cleaning Up......." << std::endl;
        
        cleanUpSwapChain();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(m_device, m_uniformBuffers[i], nullptr);
            vkFreeMemory(m_device, m_uniformBuffersMemory[i], nullptr);
        }

		vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);

		destroyAllObjects();
		
		for (int i = 0; i < m_imageAvailableSemaphores.size(); i++) vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr); 
		for (int i = 0; i < m_renderFinishedSemaphores.size(); i++) vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
		for (int i = 0; i < m_inFlightFences.size(); i++) vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
		    

        
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);


		vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
		

		vkDestroySurfaceKHR(m_vkInstance, m_surface, nullptr);
        vkDestroyDevice(m_device, nullptr);


		if (m_enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);
        }

        vkDestroyInstance(m_vkInstance, nullptr);

        glfwDestroyWindow(m_window);

        glfwTerminate();
		
		std::cout << "Cleaned Up." << std::endl;
	}

	

}
