#include "commandbuffers.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"
#include "../surface/windowsurface.hpp"
#include "../../defines.hpp"

namespace gloria::core {
	void CommandPool::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface) {
		QueueFamilyIndices indices(physicalDevice, surface);

		VkCommandPoolCreateInfo poolInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = indices.getGraphicsFamily().value()
		};

		VK_VALIDATE(vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS, "Failed to create command pool");
	}

	void CommandPool::destroy(VkDevice device) {
		vkDestroyCommandPool(device, m_commandPool, nullptr);
	}

	void CommandBuffer::createCommandBuffer(VkDevice device, CommandPool pool) {
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = pool.getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		VK_VALIDATE(vkAllocateCommandBuffers(device, &allocInfo, &m_commandBuffer) != VK_SUCCESS, "Failed to allocate command buffers");
	}

	void CommandBuffer::recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, Swapchain swapchain, std::uint32_t imageIndex) {
		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr
		};

		VK_VALIDATE(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS, "Failed to begin recording command buffer");

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

		VkViewport viewport = {
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapchain.getExtent().width),
			.height = static_cast<float>(swapchain.getExtent().width),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = swapchain.getExtent()
		};

		vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

		beginFrame(swapchain.m_swapchainImages[imageIndex], swapchain, m_commandBuffer, imageIndex);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		endFrame(swapchain.m_swapchainImages[imageIndex], swapchain, m_commandBuffer, imageIndex);
	}

	void CommandBuffer::beginFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
		VkImageMemoryBarrier imageMemoryBarrier = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.image = swapchainImage,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		VkRenderingAttachmentInfoKHR colorAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
		.imageView = swapchain.m_swapchainImageViews[imageIndex],
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue = {
			.color = { 0.0f, 0.0f, 0.0f, 0.0f }
		}
		};

		const VkRenderingInfoKHR renderingInfo = {
			   .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
			   .renderArea = { 0, 0, swapchain.getExtent().width, swapchain.getExtent().height},
			   .layerCount = 1,
			   .colorAttachmentCount = 1,
			   .pColorAttachments = &colorAttachment,
			   // .pDepthAttachment = &m_DepthAttachment // if not specified, commen this line out.
		};

		vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
	}

	void CommandBuffer::endFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
		vkCmdEndRendering(m_commandBuffer);

		const VkImageMemoryBarrier imageMemoryBarrier = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.image = swapchainImage,
			.subresourceRange = {
				 .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // srcStageMask
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // dstStageMask
			0,
			0,
			nullptr,
			0,
			nullptr,
			1, // imageMemoryBarrierCount
			&imageMemoryBarrier // pImageMemoryBarriers
		);
	}
}