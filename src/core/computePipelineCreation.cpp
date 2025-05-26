#include "core/core.hpp"
#include <vulkan/vulkan.h>
#include "renderer/graphicsPipelineUtils.hpp"

namespace Core{

	void FluidSim::createComputePipeline(){
		auto computeShaderCode = Renderer::readFile("shaders/compute.spv");

		VkShaderModule computeShaderModule = Renderer::createShaderModule(m_device,computeShaderCode);

		VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
		computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		computeShaderStageInfo.module = computeShaderModule;
		computeShaderStageInfo.pName = "main";
	}
}
