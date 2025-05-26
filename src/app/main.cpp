#include "core/core.hpp"
#include "renderer/renderer.hpp"

void Update(){
}

int main(){

	Renderer::VulkanRender vkRenderer;

	Core::FluidSim(vkRenderer.getVulkanContext());
	
	vkRenderer.StartLoop(Update);
	
	
}
