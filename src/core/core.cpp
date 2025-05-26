#include "core/core.hpp"

namespace Core {
	FluidSim::FluidSim(Renderer::VulkanContext Context){
			m_instance = Context.m_vkInstance;
			m_physicalDevice = Context.m_physicalDevice;
			m_device = Context.m_device;
			m_surface = Context.m_surface;
			m_window = Context.m_window;
	}
}
