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
		SwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		~SwapchainSupport();

		SwapchainSupportDetails getSwapchainSupportDetails();
		SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		// check for our wanted surface format, if it cant be found we just return the best one.
		VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		// check for our wanted presentmode, if it cant be found we just return the standard one that always exists.
		VkPresentModeKHR selectSwapPresentMode(const std::vector< VkPresentModeKHR>& availablePresentModes);

		VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		SwapchainSupportDetails m_details;
	};
}