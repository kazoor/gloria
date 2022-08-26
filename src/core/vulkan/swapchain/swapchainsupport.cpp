#include "swapchainsupport.hpp"
#include <vector>
#include <iostream>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace gloria::core {
	SwapchainSupport::SwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		mDetails = querySwapchainSupport(device, surface);
	}

	SwapchainSupport::~SwapchainSupport() {}

	SwapchainSupportDetails SwapchainSupport::getSwapchainSupportDetails() {
		return mDetails;
	}

	SwapchainSupportDetails SwapchainSupport::querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		std::uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		std::uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	// check for our wanted surface format, if it cant be found we just return the best one.
	VkSurfaceFormatKHR SwapchainSupport::selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	// check for our wanted presentmode, if it cant be found we just return the standard one that always exists.
	VkPresentModeKHR SwapchainSupport::selectSwapPresentMode(const std::vector< VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapchainSupport::selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
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
}