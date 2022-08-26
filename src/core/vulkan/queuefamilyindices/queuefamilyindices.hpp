#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <optional>
#include <vector>
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	class QueueFamilyIndices {
	public:
		QueueFamilyIndices(VkPhysicalDevice device, WindowSurface surface) {
			findQueueFamilies(device, surface);
		}

		void findQueueFamilies(VkPhysicalDevice device, WindowSurface surface) {
			std::uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& family : queueFamilies) {
				if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					m_graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.getSurface(), &presentSupport);

				if (presentSupport) {
					m_presentFamily = i;
				}

				if (isComplete())
					break;

				++i;
			}
		}

		bool isComplete() {
			return m_graphicsFamily.has_value() && m_presentFamily.has_value();
		}

		/*std::optional<std::uint32_t> getGraphicsFamily()& {
			return m_graphicsFamily;
		}

		std::optional<std::uint32_t> getPresentFamily()& {
			return m_presentFamily;
		}*/

	public:
		std::optional<std::uint32_t> m_graphicsFamily;
		std::optional<std::uint32_t> m_presentFamily;
	};
}