#include "commandbuffers.hpp"
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "../physicaldevice/physicaldevice.hpp"
#include "../../core/instance/instance.hpp"

namespace gloria::vk {
	CommandPool::CommandPool() {
	}

	CommandPool::~CommandPool() {
	}

	void CommandPool::init() {
		createCommandPool();
	}

	void CommandPool::createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(core::Instance::get().getVkInstance().getPhysicalDevice().get());

		VkCommandPoolCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value()
		};

		VK_VALIDATE(vkCreateCommandPool(core::Instance::get().getVkInstance().getLogicalDevice().get(), &createInfo, nullptr, &mCommandPool), "Failed to create command pool");

#ifdef DEBUG
		if (mCommandPool != VK_NULL_HANDLE)
			GL_CORE_INFO("Created command pool");
#endif // DEBUG
	}

	void CommandPool::destroy() {
		vkDestroyCommandPool(core::Instance::get().getVkInstance().getLogicalDevice().get(), mCommandPool, nullptr);
	}

	VkCommandPool CommandPool::get() {
		return mCommandPool;
	}

	CommandBuffer::CommandBuffer() {
	}

	CommandBuffer::~CommandBuffer() {
	}

	void CommandBuffer::init() {
		createCommandBuffer();
	}

	void CommandBuffer::createCommandBuffer() {
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = core::Instance::get().getVkInstance().getCommandPool().get(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		VK_VALIDATE(vkAllocateCommandBuffers(core::Instance::get().getVkInstance().getLogicalDevice().get(), &allocInfo, &mCommandBuffer), "Failed to create command buffers");

#ifdef DEBUG
		if (mCommandBuffer != VK_NULL_HANDLE)
			GL_CORE_INFO("Created command buffer");
#endif // DEBUG
	}

	void CommandBuffer::destroy() {
	}

	VkCommandBuffer& CommandBuffer::get() {
		return mCommandBuffer;
	}
}