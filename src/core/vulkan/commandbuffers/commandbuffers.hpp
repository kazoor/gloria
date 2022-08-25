#pragma once
#include <vulkan/vulkan.h>
#include "../surface/windowsurface.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"

namespace gloria::core {
	class CommandPool {
	public:
		CommandPool() {}

		CommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface) {
			createCommandPool(device, physicalDevice, surface);
		}

		~CommandPool() {}

		void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface);

		void destroy(VkDevice device);

		VkCommandPool getCommandPool() {
			return m_commandPool;
		}

	private:
		VkCommandPool m_commandPool;
	};

	class CommandBuffer {
	public:
		CommandBuffer() {}

		CommandBuffer(VkDevice device, CommandPool pool) {
			createCommandBuffer(device, pool);
		}

		~CommandBuffer() {}

		void createCommandBuffer(VkDevice device, CommandPool pool);

		void recordCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline pipeline, Swapchain swapchain, std::uint32_t imageIndex);

		void beginFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);

		void endFrame(VkImage swapchainImage, Swapchain swapchain, VkCommandBuffer commandBuffer, std::uint32_t imageIndex);
	private:
		VkCommandBuffer m_commandBuffer;
	};
}