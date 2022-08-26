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
			return mPipeline;
		}

		void destroy(VkDevice device);

	private:
		VkViewport mViewport;
		VkPipelineLayout mPipelineLayout;
		VkPipeline mPipeline;
	};
}