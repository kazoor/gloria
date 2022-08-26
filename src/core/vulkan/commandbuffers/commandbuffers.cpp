#include "commandbuffers.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../graphicspipeline/graphicspipeline.hpp"
#include "../swapchain/swapchain.hpp"
#include "../surface/windowsurface.hpp"
#include "../../defines.hpp"

namespace gloria::core {
	CommandPool::CommandPool() {}

	CommandPool::CommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface) {
		createCommandPool(device, physicalDevice, surface);
	}

	CommandPool::~CommandPool() {}

	void CommandPool::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, WindowSurface surface) {
		QueueFamilyIndices indices(physicalDevice, surface);

		VkCommandPoolCreateInfo poolInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = indices.m_graphicsFamily.value()
		};

		VK_VALIDATE(vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool), "Failed to create command pool");
	}

	void CommandPool::destroy(VkDevice device) {
		vkDestroyCommandPool(device, m_commandPool, nullptr);
	}

	CommandBuffer::CommandBuffer() {}

	CommandBuffer::CommandBuffer(VkDevice device, CommandPool pool) {
		createCommandBuffer(device, pool);
	}

	CommandBuffer::~CommandBuffer() {}

	void CommandBuffer::createCommandBuffer(VkDevice device, CommandPool pool) {
		m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = pool.getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<std::uint32_t>(m_commandBuffers.size())
		};

		VK_VALIDATE(vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffers.data()), "Failed to allocate command buffers");
	}
}