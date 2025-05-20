#include "renderer/mesh.hpp" 
#include "renderer/bufferUtils.hpp"
#include <vulkan/vulkan_core.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"
#include <cstring>
#include <stdexcept>
#include <string>

namespace Renderer {

	void Mesh::generateImageInfos(VkDescriptorImageInfo &albedoMapImageInfo, VkDescriptorImageInfo &normalMapImageInfo){
		albedoMapImageInfo = m_albedoMap.getImageInfo();
		normalMapImageInfo = m_normalMap.getImageInfo();
	}

	void Mesh::CalculateTangents(std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices){
        for (int i = 0; i < indices.size(); i+=3) {
            uint32_t i0 = indices[i];
            uint32_t i1 = indices[i+1];
            uint32_t i2 = indices[i+2];

            glm::vec3 e1 = vertices[i1].pos - vertices[i0].pos;
            glm::vec3 e2 = vertices[i2].pos - vertices[i0].pos;

            float deltaU1 = vertices[i1].texCoord.x - vertices[i0].texCoord.x;
            float deltaV1 = vertices[i1].texCoord.y - vertices[i0].texCoord.y;

            float deltaU2 = vertices[i2].texCoord.x - vertices[i0].texCoord.x;
            float deltaV2 = vertices[i2].texCoord.y - vertices[i0].texCoord.y;

            float divider = 1.0f/(deltaU1 * deltaV2 - deltaU2 * deltaV1);

            glm::vec3 tangent = glm::normalize(divider * (deltaV2 * e1 - deltaV1 * e2));

            vertices[i0].tangent = tangent;
            vertices[i1].tangent = tangent;
            vertices[i2].tangent = tangent;


        }
    }

	void Mesh::LoadModel(const std::string folderPath,VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool){
		std::string modelPath = folderPath + "/mesh.obj";
		std::string albedoMapPath = folderPath + "/albedoMap.png";
		std::string normalMapPath = folderPath + "/normalMap.png";
		

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };
                
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
                
                vertex.color = {1.0f, 1.0f, 1.0f};
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]

                };          

                m_vertices.push_back(vertex);
                m_indices.push_back(m_indices.size());
            }
        }

        CalculateTangents(m_vertices, m_indices);
	
		createAllBuffers(device, physcialDevice, graphicsQueue, commandPool);
		m_albedoMap.createTexture(device, physcialDevice, commandPool, graphicsQueue, albedoMapPath, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);	
		m_normalMap.createTexture(device, physcialDevice, commandPool, graphicsQueue, normalMapPath, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);	
		

				
	}

	void Mesh::DestroyObject(VkDevice device){

		m_albedoMap.DestroyTexture(device);
		m_normalMap.DestroyTexture(device);
		
		vkDestroyBuffer(device, m_indexBuffer, nullptr);
        vkFreeMemory(device, m_indexBufferMemory, nullptr);


        vkDestroyBuffer(device, m_vertexBuffer, nullptr);
        vkFreeMemory(device, m_vertexBufferMemory, nullptr);

	}

	void Mesh::createAllBuffers(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool){
		
		createVertexBuffer(device, physcialDevice,graphicsQueue,commandPool);
		createIndexBuffer(device, physcialDevice,graphicsQueue,commandPool);
		
	}

	void Mesh::createVertexBuffer(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool){
		VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        createBuffer(device, physcialDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, m_vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);
		

        
        createBuffer(device, physcialDevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

        copyBuffer(device,stagingBuffer,m_vertexBuffer,bufferSize,graphicsQueue,commandPool);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);	
	}

	void Mesh::createIndexBuffer(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool){

		VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(device, physcialDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, m_indices.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(device, physcialDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

        copyBuffer(device, stagingBuffer, m_indexBuffer, bufferSize, graphicsQueue, commandPool);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
		
	}
}
