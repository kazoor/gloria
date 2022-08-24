#pragma once
#undef max
#include <vector>
#include <iostream>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace gloria::core {
	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR m_capabilities;
		std::vector<VkSurfaceFormatKHR> m_formats;
		std::vector<VkPresentModeKHR> m_presentModes;
	};

	// Class that checks all the swapchain properties
	class SwapchainSupport {
	public:
		SwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
			m_details = querySwapchainSupport(device, surface);
		}

		~SwapchainSupport() {}

		SwapchainSupportDetails getSwapchainSupportDetails() {
			return m_details;
		}

		SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
			SwapchainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.m_capabilities);

			std::uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.m_formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.m_formats.data());
			}

			std::uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.m_presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.m_presentModes.data());
			}

			return details;
		}

		// check for our wanted surface format, if it cant be found we just return the best one.
		VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			for (const auto& availableFormat : availableFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		// check for our wanted presentmode, if it cant be found we just return the standard one that always exists.
		VkPresentModeKHR selectSwapPresentMode(const std::vector< VkPresentModeKHR>& availablePresentModes) {
			for (const auto& availablePresentMode : availablePresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
				return capabilities.currentExtent;
			}
			else {
				int width, height;

				glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

				VkExtent2D correctedExtent = {
					static_cast<std::uint32_t>(width),
					static_cast<std::uint32_t>(height)
				};

				// clamp the values between the possible min, max
				correctedExtent.width = std::clamp(correctedExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				correctedExtent.height = std::clamp(correctedExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return correctedExtent;
			}
		}

	private:
		SwapchainSupportDetails m_details;
	};
}