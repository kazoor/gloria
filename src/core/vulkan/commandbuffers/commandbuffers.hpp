#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "../surface/windowsurface.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"

namespace gloria::core {
	class CommandPool {
	public:
		CommandPool();

		CommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface);

		~CommandPool();

		void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface);

		void destroy(VkDevice device);

		VkCommandPool getCommandPool() {
			return mCommandPool;
		}

	private:
		VkCommandPool mCommandPool;
	};

	class CommandBuffer {
	public:
		CommandBuffer();

		CommandBuffer(VkDevice device, CommandPool pool);

		~CommandBuffer();

		void createCommandBuffer(VkDevice device, CommandPool pool);

		std::vector<VkCommandBuffer> CommandBuffers;
	};
}