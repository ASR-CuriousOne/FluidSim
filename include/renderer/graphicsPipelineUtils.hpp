#ifndef GRAPHICS_PIPELINE_UTILS_HPP
#define GRAPHICS_PIPELINE_UTILS_HPP
#include <string>
#include <vector>
#include <vulkan/vulkan.h>


namespace Renderer {
	void createRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkRenderPass &renderPass, VkFormat swapChainImageFormat);

	std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

}

#endif
