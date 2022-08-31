#pragma once
#include <vulkan/vulkan.h>
#include "../swapchain/swapchain.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"

namespace gloria::vk {
	class VulkanRenderer {
	public:
		VulkanRenderer();

		~VulkanRenderer();

		void draw();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, SwapChain swapchain, std::uint32_t imageIndex);

		void beginFrame(VkImage swapchainImage, SwapChain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);

		void endFrame(VkImage swapchainImage, SwapChain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);
	};
}