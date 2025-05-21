#include "renderer/renderer.hpp"
#include "renderer/uniformBufferObject.hpp"
#include "renderer/bufferUtils.hpp"
#include <cstdint>
#include <cstring>
#include <glm/ext/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <stdexcept>
namespace Renderer {
	void VulkanRender::createDescriptorSetLayout(){
		

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerLayoutBindingAlbedo{};
        samplerLayoutBindingAlbedo.binding = 1;
        samplerLayoutBindingAlbedo.descriptorCount = 1;
        samplerLayoutBindingAlbedo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBindingAlbedo.pImmutableSamplers = nullptr;
        samplerLayoutBindingAlbedo.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBindingNormal{};
        samplerLayoutBindingNormal.binding = 2;
        samplerLayoutBindingNormal.descriptorCount = 1;
        samplerLayoutBindingNormal.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBindingNormal.pImmutableSamplers = nullptr;
        samplerLayoutBindingNormal.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


        std::array<VkDescriptorSetLayoutBinding, 3> bindings = {uboLayoutBinding, samplerLayoutBindingAlbedo, samplerLayoutBindingNormal};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

	void VulkanRender::createUniformBuffers(){
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		pushConstants.resize(MAX_FRAMES_IN_FLIGHT);

        m_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
        m_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
        m_uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);

        for (size_t i = 0; i < m_uniformBuffers.size(); i++) {
            createBuffer(m_device,m_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i] );

            vkMapMemory(m_device, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
        }
    }

	void VulkanRender::updateUniformBuffer(uint32_t currentImage, uint32_t currentObject) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        
        camera.position = {
            radius * cos(glm::radians(azimuth )) * sin(glm::radians(altitude)),
            radius * sin(glm::radians(azimuth )) * sin(glm::radians(altitude)),
            radius * cos(glm::radians(altitude))
        }; 

		glm::vec4 LightPosAndPower = {
			100.0f * cos(time * glm::radians(-6.0f)) * sin(glm::radians(90.0f)),
            100.0f * sin(time * glm::radians(-6.0f)) * sin(glm::radians(90.0f)),
            100.0f * cos(glm::radians(90.0f)), 1000.0f	
		};

        ubo.model = glm::rotate(glm::translate(glm::mat4(1.0f), m_allObjects[currentObject].getPosition()),glm::radians(20.0f) * time,glm::vec3(0.0,0.0,1.0f));
        pushConstants[currentImage].view = glm::lookAt(camera.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		pushConstants[currentImage].proj = glm::perspective(glm::radians(camera.FOV), m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1f, 100.0f);
        pushConstants[currentImage].LightDirAndPower = LightPosAndPower;
		pushConstants[currentImage].LightColor = {0.9,1.0,0.9};
        pushConstants[currentImage].proj[1][1] *= -1;

        memcpy(m_uniformBuffersMapped[currentImage * NUM_OF_OBJECTS + currentObject], &ubo, sizeof(ubo));

		
    }
    
    void VulkanRender::createDescriptorPool(){
        std::array<VkDescriptorPoolSize, 3> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
        poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);

        

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);

        if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

    }

    void VulkanRender::createDescriptorSets() {

        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS, m_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
        allocInfo.pSetLayouts = layouts.data();

        m_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS);
        if (vkAllocateDescriptorSets(m_device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }        
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT * NUM_OF_OBJECTS; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo albedoMapImageInfo{};
			VkDescriptorImageInfo normalMapImageInfo{};

			m_allObjects[i % NUM_OF_OBJECTS].generateImageInfos(albedoMapImageInfo,normalMapImageInfo);

            std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = m_descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;
            
            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = m_descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &albedoMapImageInfo;

            descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[2].dstSet = m_descriptorSets[i];
            descriptorWrites[2].dstBinding = 2;
            descriptorWrites[2].dstArrayElement = 0;
            descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[2].descriptorCount = 1;
            descriptorWrites[2].pImageInfo = &normalMapImageInfo;


            vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        }

    }
}
