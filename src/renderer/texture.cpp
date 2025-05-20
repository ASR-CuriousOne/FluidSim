#include "renderer/texture.hpp"
#include "renderer/bufferUtils.hpp"
#include "renderer/imageUtils.hpp"
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


namespace Renderer{
	
	void Texture::createTexture(VkDevice device,VkPhysicalDevice physicalDevice,VkCommandPool commandPool,VkQueue graphicsQueue,
			const std::string& path,VkFormat imageFormat,VkImageAspectFlags imageAspectFlags){
		createTextureImage(device, physicalDevice, commandPool, graphicsQueue, path, imageFormat);
		createImageView(device, imageFormat, imageAspectFlags);
		createTextureSampler(device, physicalDevice);
		
	}

	void Texture::createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, 
				const std::string& texturePath,VkFormat ImageFormat){

		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(device, physicalDevice,imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels);


		createImage(device, physicalDevice,texWidth, texHeight, ImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
				m_image, m_memoryRequirements);

		transitionImageLayout(device,commandPool,graphicsQueue, m_image, ImageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer, m_image, device,commandPool,graphicsQueue, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(device,commandPool,graphicsQueue,m_image, ImageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

	}

	void Texture::createImageView(VkDevice device, VkFormat imageFormat, VkImageAspectFlags imageAspectFlags){
		m_imageView = createImageViewForImage(device,m_image,imageFormat,imageAspectFlags);
	}

	void Texture::createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice){
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
           throw std::runtime_error("failed to create texture sampler!");
        }
		
    }

	void Texture::DestroyTexture(VkDevice device){
		vkDestroySampler(device, m_sampler, nullptr);
        vkDestroyImageView(device, m_imageView, nullptr);

        vkDestroyImage(device,m_image, nullptr);
        vkFreeMemory(device, m_memoryRequirements, nullptr);
	}

	VkDescriptorImageInfo Texture::getImageInfo(){
		VkDescriptorImageInfo imageInfo{};

		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_imageView;
		imageInfo.sampler = m_sampler;

		return imageInfo;

	}
}
