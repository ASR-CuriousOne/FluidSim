#include "renderer/renderer.hpp"
#include "renderer/imageUtils.hpp"

namespace Renderer {
	void VulkanRender::createDepthResources() {
        VkFormat depthFormat = findDepthFormat(m_physicalDevice);

        createImage(m_device,m_physicalDevice,m_swapChainExtent.width, m_swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, 
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);

        m_depthImageView = createImageViewForImage(m_device,m_depthImage, depthFormat,VK_IMAGE_ASPECT_DEPTH_BIT);

        transitionImageLayout(m_device,m_commandPool,m_graphicsQueue,m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }	
}
