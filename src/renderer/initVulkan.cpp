#include "renderer/renderer.hpp"
#include "renderer/validationLayers.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <iostream>


namespace Renderer{
	void VulkanRender::InitializeVulkan(){
		createInstance();
		if(m_enableValidationLayers) 
			setupDebugMessenger(m_vkInstance, m_debugMessenger); 	
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createDescriptorSetLayout();
		createGraphicsPipeline();
		createCommandPool();
		createDepthResources();
		createFrameBuffers();
		createAllObjects();
		createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
		createCommandBuffers();
		createSyncObjects();
	}

	void VulkanRender::createInstance(){
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "HelloTriangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
    
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        if (m_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            createInfo.ppEnabledLayerNames = m_validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = &debugCreateInfo;
        } 
        else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }


        if (m_enableValidationLayers && !checkValidationLayerSupport(m_validationLayers)) {
            throw std::runtime_error("validation layers requested, but not available!");
        }
    
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        
 
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        auto extensions = getRequiredExtensions(m_enableValidationLayers);
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
    
        if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS){
            throw std::runtime_error("failed to create instance!");
        }

	}

	VulkanContext VulkanRender::getVulkanContext(){
		VulkanContext context{};
		context.m_vkInstance = m_vkInstance;
		context.m_physicalDevice = m_physicalDevice;
		context.m_device = m_device;
		context.m_surface = m_surface;
		context.m_window = m_window;

		return context;
	}
}
