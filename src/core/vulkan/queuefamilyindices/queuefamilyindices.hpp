#pragma once
#include <vulkan/vulkan.h>
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	class QueueFamilyIndices {
	public:
		QueueFamilyIndices(VkPhysicalDevice device, WindowSurface surface);

		void findQueueFamilies(VkPhysicalDevice device, WindowSurface surface);

		bool isComplete(); 

	public:
		std::optional<std::uint32_t> graphicsFamily;
		std::optional<std::uint32_t> presentFamily;
	};
}