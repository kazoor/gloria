#include "vulkanrenderer.hpp"
#include <vulkan/vulkan.h>
#include <iostream>
#include "../../defines.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"
#include "../commandbuffers/commandbuffers.hpp"
#include "../logicaldevice/logicaldevice.hpp"

namespace gloria::core {
	VulkanRenderer::VulkanRenderer() {

	}

	VulkanRenderer::VulkanRenderer(const LogicalDevice& device, const Swapchain& swapchain, const GraphicsPipeline& pipeline, const CommandBuffer& commandBuffer) : mDevice(device), mSwapchain(swapchain), mPipeline(pipeline), mCommandBuffer(commandBuffer) {

	}

	VulkanRenderer::~VulkanRenderer() {

	}

	void VulkanRenderer::drawFrame() {
		vkWaitForFences(mDevice.getDevice(), 1, &mSwapchain.inFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(mDevice.getDevice(), 1, &mSwapchain.inFlightFences[mCurrentFrame]);

		std::uint32_t imageIndex;
		vkAcquireNextImageKHR(mDevice.getDevice(), mSwapchain.getSwapchain(), UINT64_MAX, mSwapchain.imageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, &imageIndex);

		// auto commandBuffer = m_commandBuffer.getCommandBuffer();
		// vkResetCommandBuffer(commandBuffer, 0);

		recordCommandBuffer(mCommandBuffer.CommandBuffers[mCurrentFrame], mPipeline, mSwapchain, imageIndex);

		VkSemaphore waitSemaphores[] = { mSwapchain.imageAvailableSemaphores[mCurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore signalSemaphores[] = { mSwapchain.renderFinishedSemaphores[mCurrentFrame] };

		VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &mCommandBuffer.CommandBuffers[mCurrentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores,
		};

		VK_VALIDATE(vkQueueSubmit(mDevice.getGraphicsQueue(), 1, &submitInfo, mSwapchain.inFlightFences[mCurrentFrame]), "Failed to submit draw command buffer");

		VkSubpassDependency dependency{};

		VkSwapchainKHR swapchains[] = { mSwapchain.getSwapchain() };
		VkPresentInfoKHR presentInfo = {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapchains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};

		vkQueuePresentKHR(mDevice.getPresentQueue(), &presentInfo);

		mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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

		beginFrame(swapchain.swapchainImages[imageIndex], swapchain, commandBuffer, imageIndex);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		endFrame(swapchain.swapchainImages[imageIndex], swapchain, commandBuffer, imageIndex);

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
		.imageView = swapchain.swapchainImageViews[imageIndex],
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