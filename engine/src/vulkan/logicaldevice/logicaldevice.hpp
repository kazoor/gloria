#pragma once
#include <vulkan/vulkan.h>

namespace gloria::vk {
	class LogicalDevice {
	public:
		LogicalDevice();

		~LogicalDevice();

		void init();

		void createLogicalDevice();

		void destroy();

		VkDevice& get();

		VkQueue& getGraphicsQueue();

	private:
		VkDevice mDevice;

		VkQueue mGraphicsQueue;

		VkQueue mPresentQueue;
	};
}