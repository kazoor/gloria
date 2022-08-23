#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <optional>
#include <vector>

namespace gloria::core {
	class QueueFamilyIndices {
	public:
		QueueFamilyIndices(VkPhysicalDevice device) {
			findQueueFamilies(device);
		}

		void findQueueFamilies(VkPhysicalDevice device) {
			std::uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& family : queueFamilies) {
				if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					m_graphicsFamily = i;
				}

				if (isComplete())
					break;

				++i;
			}
		}

		bool isComplete() {
			return m_graphicsFamily.has_value();
		}

		std::optional<std::uint32_t> getGraphicsFamily() {
			return m_graphicsFamily;
		}

	private:
		std::optional<uint32_t> m_graphicsFamily;
	};
}