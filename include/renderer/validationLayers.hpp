#ifndef VALIDATION_LAYERS_HPP
#define VALIDATION_LAYERS_HPP
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Renderer {
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

	void setupDebugMessenger(VkInstance VkInstance, VkDebugUtilsMessengerEXT &debugMessenger);		

	VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance, 
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
            const VkAllocationCallbacks* pAllocator, 
            VkDebugUtilsMessengerEXT* pDebugMessenger);

	bool checkValidationLayerSupport(const std::vector<const char*> validatitionLayers);

	std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		
	
	
	
}
#endif
