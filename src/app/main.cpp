#include "core/core.hpp"
#include "renderer/renderer.hpp"

void Update(){
}

int main(){
	Core::Application app;	

	Renderer::VulkanRender vkRenderer;
	
	vkRenderer.StartLoop(Update);
	
	
}
