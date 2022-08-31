#include "vulkanrenderer.hpp"
#include "../../core/instance/instance.hpp"
#include "../../defines.hpp"

namespace gloria::vk {
	VulkanRenderer::VulkanRenderer() {
	}

	VulkanRenderer::~VulkanRenderer() {
	}

	void VulkanRenderer::draw() {
		vkWaitForFences(core::Instance::get().getVkInstance().getLogicalDevice().get(), 1, &core::Instance::get().getVkInstance().getSwapchain().inFlightFences[core::Instance::get().currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(core::Instance::get().getVkInstance().getLogicalDevice().get(), 1, &core::Instance::get().getVkInstance().getSwapchain().inFlightFences[core::Instance::get().currentFrame]);

		std::uint32_t imageIndex;
		vkAcquireNextImageKHR(core::Instance::get().getVkInstance().getLogicalDevice().get(), core::Instance::get().getVkInstance().getSwapchain().get(), UINT64_MAX, core::Instance::get().getVkInstance().getSwapchain().imageAvailableSemaphores[core::Instance::get().currentFrame], VK_NULL_HANDLE, &imageIndex);

		recordCommandBuffer(core::Instance::get().getVkInstance().getCommandBuffer().mCommandBuffers[core::Instance::get().currentFrame], core::Instance::get().getVkInstance().getPipeline(), core::Instance::get().getVkInstance().getSwapchain(), imageIndex);

		VkSemaphore waitSemaphores[] = { core::Instance::get().getVkInstance().getSwapchain().imageAvailableSemaphores[core::Instance::get().currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore signalSemaphores[] = { core::Instance::get().getVkInstance().getSwapchain().renderFinishedSemaphores[core::Instance::get().currentFrame] };

		VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &core::Instance::get().getVkInstance().getCommandBuffer().mCommandBuffers[core::Instance::get().currentFrame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores
		};

		VK_VALIDATE(vkQueueSubmit(core::Instance::get().getVkInstance().getLogicalDevice().getGraphicsQueue(), 1, &submitInfo, core::Instance::get().getVkInstance().getSwapchain().inFlightFences[core::Instance::get().currentFrame]), "Failed to submit draw command buffer");

		VkSwapchainKHR swapchains[] = { core::Instance::get().getVkInstance().getSwapchain().get() };
		VkPresentInfoKHR presentInfo = {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapchains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};

		vkQueuePresentKHR(core::Instance::get().getVkInstance().getLogicalDevice().getPresentQueue(), &presentInfo);

		core::Instance::get().currentFrame = (core::Instance::get().currentFrame + 1) % core::Instance::get().getVkInstance().getSwapchain().MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, SwapChain swapchain, std::uint32_t imageIndex) {
		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr
		};

		VK_VALIDATE(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer");

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

		VkViewport viewport = {
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapchain.swapChainExtent.width),
			.height = static_cast<float>(swapchain.swapChainExtent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = swapchain.swapChainExtent
		};

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		beginFrame(swapchain.swapChainImages[imageIndex], swapchain, commandBuffer, imageIndex);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		endFrame(swapchain.swapChainImages[imageIndex], swapchain, commandBuffer, imageIndex);

		VK_VALIDATE(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer");
	}

	void VulkanRenderer::beginFrame(VkImage swapchainImage, SwapChain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
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
		.imageView = swapchain.swapChainImageViews[imageIndex],
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue = {
			.color = { 0.0f, 0.0f, 0.0f, 0.0f }
			}
		};

		const VkRenderingInfoKHR renderingInfo = {
			   .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
			   .renderArea = { 0, 0, swapchain.swapChainExtent.width, swapchain.swapChainExtent.height},
			   .layerCount = 1,
			   .colorAttachmentCount = 1,
			   .pColorAttachments = &colorAttachment,
			   // .pDepthAttachment = &m_DepthAttachment // if not specified, commen this line out.
		};

		vkCmdBeginRendering(commandBuffer, &renderingInfo);
	}

	void VulkanRenderer::endFrame(VkImage swapchainImage, SwapChain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex) {
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