#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <renderer/mesh.hpp>
#include <renderer/camera.hpp>
#include <renderer/pushConstantStruct.hpp>



namespace Renderer{ 
	class VulkanRender{

		GLFWwindow* m_window;

		VkInstance m_vkInstance;
		
		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

		const std::vector<const char*> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> m_deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

#ifndef NDEBUG
		bool m_enableValidationLayers = true;

#else
		bool m_enableValidationLayers = false;
#endif

		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;

		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkImageView> m_swapChainImageViews;
		const int MAX_FRAMES_IN_FLIGHT = 2;

		VkDescriptorSetLayout m_descriptorSetLayout;
        VkPipelineLayout m_pipelineLayout;
        VkRenderPass m_renderPass;
        VkPipeline m_graphicsPipeline;

		std::vector<VkFramebuffer> m_swapChainFramebuffers;

		std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<void*> m_uniformBuffersMapped;
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;

		std::vector<MeshPushConstants> pushConstants;

		float azimuth = 0 , altitude = 60.0f , radius = 4.0f;

        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        uint32_t currentFrame = 0;
        bool framebufferResized = false;

		VkImage m_depthImage;
        VkDeviceMemory m_depthImageMemory;
        VkImageView m_depthImageView;

		const int NUM_OF_OBJECTS = 2;
		std::vector<Mesh> m_allObjects;
	

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		
		Camera camera;

		
	public:
		VulkanRender();
		~VulkanRender();

		void InitializeWindow();
		void InitializeVulkan();

		void createInstance();
		
		void StartLoop(void(*Update)());

		void createSurface();

		void pickPhysicalDevice();
		void createLogicalDevice();

		void createSwapChain();
		void reCreateSwapChain();
		void cleanUpSwapChain(); 

		void createImageViews();

		void createRenderPass();
		void createGraphicsPipeline();

		void createCommandPool();
		void createCommandBuffers();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void createAllObjects();
		void destroyAllObjects();

		void createDepthResources();

		void createFrameBuffers();

		void createSyncObjects();

		void createDescriptorSetLayout();
		void createUniformBuffers();
		void createDescriptorPool();
		void createDescriptorSets();
		void updateUniformBuffer(uint32_t currentFrame, uint32_t currentObject);

		void drawFrame();







		static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<VulkanRender*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
        }
		
	};
}

#endif
