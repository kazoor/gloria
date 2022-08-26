#include "queuefamilyindices.hpp"
#include <iostream>
#include <optional>
#include <vector>
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	QueueFamilyIndices::QueueFamilyIndices(VkPhysicalDevice device, WindowSurface surface) {
		findQueueFamilies(device, surface);
	}

	void QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device, WindowSurface surface) {
		std::uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& family : queueFamilies) {
			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.getSurface(), &presentSupport);

			if (presentSupport) {
				presentFamily = i;
			}

			if (isComplete())
				break;

			++i;
		}
	}

	bool QueueFamilyIndices::isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

}