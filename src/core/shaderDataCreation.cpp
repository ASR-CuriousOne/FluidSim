#include "core/core.hpp"

namespace Core{
	void FluidSim::createShaderStorageBuffers(){
		shaderStorageBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		shaderStorageBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

	
	}

	void FluidSim::createAllTextures(){
		velocityPressureAndDye.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < velocityPressureAndDye.size(); i++)
		{
			velocityPressureAndDye[i].createTexture(m_device,m_physicalDevice,)
		}

	}
}
