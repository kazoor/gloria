#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace gloria::vk {
	class CommandPool {
	public:
		CommandPool();

		~CommandPool();

		void init();

		void createCommandPool();

		void destroy();

		VkCommandPool get();

	private:
		VkCommandPool mCommandPool;
	};

	class CommandBuffer {
	public:
		CommandBuffer();

		~CommandBuffer();

		void init();

		void createCommandBuffer();

		void destroy();

		std::vector<VkCommandBuffer> mCommandBuffers;
	};
}