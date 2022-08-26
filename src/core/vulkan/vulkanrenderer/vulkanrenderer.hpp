#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"
#include "../commandbuffers/commandbuffers.hpp"
#include "../logicaldevice/logicaldevice.hpp"

namespace gloria::core {
	class VulkanRenderer {
	public:
		VulkanRenderer();

		VulkanRenderer(const LogicalDevice& device, const Swapchain& swapchain, const GraphicsPipeline& pipeline, const CommandBuffer& commandbuffer);

		~VulkanRenderer();

		void drawFrame();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, Swapchain swapchain, std::uint32_t imageIndex);

		void beginFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);

		void endFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);

	private:
		LogicalDevice m_device;
		Swapchain m_swapchain;
		CommandBuffer m_commandBuffer;
		GraphicsPipeline m_pipeline;
		std::uint32_t m_currentFrame = 0;
	};
}