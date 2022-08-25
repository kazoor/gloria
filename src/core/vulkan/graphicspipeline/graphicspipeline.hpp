#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../swapchain/swapchain.hpp"

namespace gloria::core {
	class GraphicsPipeline {
	public:
		GraphicsPipeline() {}

		GraphicsPipeline(VkDevice device, Swapchain swapchain) {
			createGraphicsPipeline(device, swapchain);
		}

		~GraphicsPipeline() {}

		void createGraphicsPipeline(VkDevice device, Swapchain swapchain);

		VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

		VkPipeline getPipeline() {
			return m_pipeline;
		}

		void destroy(VkDevice device);
	private:
		VkViewport m_viewport;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;
	};
}