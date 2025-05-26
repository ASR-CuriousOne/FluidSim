#include "core/core.hpp"

namespace Core{
	void FluidSim::createShaderStorageBuffers(){
		shaderStorageBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		shaderStorageBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

	
	}
}
