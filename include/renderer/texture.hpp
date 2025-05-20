#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <vulkan/vulkan.h>

namespace Renderer {
	class Texture{
		VkImage m_image;
		VkDeviceMemory m_memoryRequirements;
		VkImageView m_imageView;
		VkSampler m_sampler;

		public:

		void createTexture(VkDevice device,VkPhysicalDevice physicalDevice,VkCommandPool commandPool,VkQueue graphicsQueue,
			const std::string& path,VkFormat imageFormat,VkImageAspectFlags imageAspectFlags);

		void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, 
				const std::string& texturePath,VkFormat ImageFormat);

		void createImageView(VkDevice device, VkFormat imageFormat, VkImageAspectFlags imageAspectFlags);
		
		void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

		void DestroyTexture(VkDevice device);

		VkDescriptorImageInfo getImageInfo();
			
	};

}
#endif
