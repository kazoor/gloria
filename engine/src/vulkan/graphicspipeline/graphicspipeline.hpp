#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace gloria::vk {
	class GraphicsPipeline {
	public:
		GraphicsPipeline();

		~GraphicsPipeline();

		void init();

		void createGraphicsPipeline();

		void destroy();

	private:
		VkShaderModule createShaderModule(const std::vector<char>& code);

		VkPipelineLayout mPipelineLayout;

		VkPipeline mPipeline;
	};
}