#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <renderer/vertex.hpp>
#include "renderer/texture.hpp"


namespace Renderer {
	class Mesh{
		glm::vec3 Position = {0.0f,0.0f,0.0f};
		glm::vec4 Rotation = {0.0f,0.0f,0.0f,0.0f};

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory; 
		
		Texture m_albedoMap;
		Texture m_normalMap;
		 
	
		public:

		VkBuffer getVertexBuffer() const {return m_vertexBuffer;}
		VkBuffer getIndexBuffer() const {return m_indexBuffer;}
		std::vector<uint32_t> getIndices() const {return m_indices;}
		std::vector<Vertex> getVertices() const {return m_vertices;}
		uint32_t getNumOfVertices() const {return static_cast<uint32_t>(m_vertices.size());}
		uint32_t getNumOfIndices() const {return static_cast<uint32_t>(m_indices.size());}

		void generateImageInfos(VkDescriptorImageInfo &albedoMapImageInfo, VkDescriptorImageInfo &normalMapImageInfo);

		void createAllBuffers(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool);
			
		void createVertexBuffer(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool);

		void createIndexBuffer(VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool);

		void CalculateTangents(std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices);

		void LoadModel(const std::string path,VkDevice device, VkPhysicalDevice physcialDevice,VkQueue graphicsQueue, VkCommandPool commandPool);

		void DestroyObject(VkDevice device);
		

	};
}
