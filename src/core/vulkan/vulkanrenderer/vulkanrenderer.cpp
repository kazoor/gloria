#include "vulkanrenderer.hpp"
#include <vulkan/vulkan.h>
#include <iostream>
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"
#include "../commandbuffers/commandbuffers.hpp"
#include "../logicaldevice/logicaldevice.hpp"

namespace gloria::core {
	VulkanRenderer::VulkanRenderer() {

	}

	VulkanRenderer::VulkanRenderer(const LogicalDevice& device, const Swapchain& swapchain, const GraphicsPipeline& pipeline, const CommandBuffer& commandBuffer) : m_device(device), m_swapchain(swapchain), m_pipeline(pipeline), m_commandBuffer(commandBuffer) {

	}

	VulkanRenderer::~VulkanRenderer() {

	}

	void VulkanRenderer::drawFrame() {
		vkWaitForFences(m_device.getDevice(), 1, &m_swapchain.m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_device.getDevice(), 1, &m_swapchain.m_inFlightFences[m_currentFrame]);

		std::uint32_t imageIndex;
		vkAcquireNextImageKHR(m_device.getDevice(), m_swapchain.getSwapchain(), UINT64_MAX, m_swapchain.m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		// auto commandBuffer = m_commandBuffer.getCommandBuffer();
		// vkResetCommandBuffer(commandBuffer, 0);

		recordCommandBuffer(m_commandBuffer.m_commandBuffers[m_currentFrame], m_pipeline, m_swapchain, imageIndex);

		VkSemaphore waitSemaphores[] = { m_swapchain.m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore signalSemaphores[] = { m_swapchain.m_renderFinishedSemaphores[m_currentFrame] };

		VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_commandBuffer.m_commandBuffers[m_currentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores
		};

		VK_VALIDATE(vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, m_swapchain.m_inFlightFences[m_currentFrame]), "Failed to submit draw command buffer");

		m_currentFrame = (m_currentFrame + 1) & MAX_FRAMES_IN_FLIGHT;

		VkSubpassDependency dependency{};

		VkSwapchainKHR swapchains[] = { m_swapchain.getSwapchain() };
		VkPresentInfoKHR presentInfo = {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapchains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};

		vkQueuePresentKHR(m_device.getPresentQueue(), &presentInfo);
	}

	void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, Swapchain swapchain, std::uint32_t imageIndex) {
		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr
		};

		VK_VALIDATE(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer");

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

		VkViewport viewport = {
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapchain.getExtent().width),
			.height = static_cast<float>(swapchain.getExtent().height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = swapchain.getExtent()
		};

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		beginFrame(swapchain.m_swapchainImages[imageIndex], swapchain, commandBuffer, imageIndex);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		endFrame(swapchain.m_swapchainImages[imageIndex], swapchain, commandBuffer, imageIndex);

		VK_VALIDATE(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer");
	}

	void VulkanRenderer::beginFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
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

		vkCmdBeginRendering(commandBuffer, &renderingInfo);
	}

	void VulkanRenderer::endFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
		vkCmdEndRendering(commandBuffer);

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