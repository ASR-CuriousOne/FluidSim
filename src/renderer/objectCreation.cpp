#include <renderer/renderer.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Renderer {
	void VulkanRender::createAllObjects(){
		m_allObjects.resize(NUM_OF_OBJECTS);

		std::fstream ObjectMeshPaths("Models/paths.txt");
		
		for (auto &&mesh : m_allObjects) {
			std::string path;
			getline(ObjectMeshPaths,path);

			if(path.empty()) std::runtime_error("Path of model not given");

 			mesh.LoadModel(path,m_device,m_physicalDevice,m_graphicsQueue,m_commandPool);
			
		}

		

		m_allObjects[0].setPosition({0.0,1.0,0.0});
		m_allObjects[1].setPosition({0.0,-1.0,0.0});

		ObjectMeshPaths.close();

		
	}

	void VulkanRender::destroyAllObjects(){
		for (auto &&mesh : m_allObjects) {
			mesh.DestroyObject(m_device);
		}

	}
}
