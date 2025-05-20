#include "renderer/renderer.hpp"
#include "renderer/imageUtils.hpp"

namespace Renderer {
	void VulkanRender::createImageViews(){
       m_swapChainImageViews.resize(m_swapChainImages.size());

        for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
            m_swapChainImageViews[i] = createImageViewForImage(m_device,m_swapChainImages[i], m_swapChainImageFormat,VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }	
}
