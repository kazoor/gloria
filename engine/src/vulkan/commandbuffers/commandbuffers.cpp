#include "commandbuffers.hpp"
#include <vulkan/vulkan.h>
#include <cstdint>
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
		mCommandBuffers.resize(core::Instance::get().getVkInstance().getSwapchain().MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = core::Instance::get().getVkInstance().getCommandPool().get(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<std::uint32_t>(mCommandBuffers.size())
		};

		VK_VALIDATE(vkAllocateCommandBuffers(core::Instance::get().getVkInstance().getLogicalDevice().get(), &allocInfo, mCommandBuffers.data()), "Failed to create command buffers");

#ifdef DEBUG
		if (!mCommandBuffers.empty())
			GL_CORE_INFO("Created command buffers");
#endif // DEBUG
	}

	void CommandBuffer::destroy() {
	}
}