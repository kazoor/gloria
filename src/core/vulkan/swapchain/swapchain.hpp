#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "swapchainsupport.hpp"
#include "../surface/windowsurface.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"

namespace gloria::core {
	class Swapchain {
	public:
		Swapchain();

		Swapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface);

		~Swapchain();
		void createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface);

		void CreateSwapchainImageViews(VkDevice device);

		void destroy(VkDevice device);

		void createSyncObjects(VkDevice device);

		VkSwapchainKHR getSwapchain();

		VkExtent2D getExtent();

		VkSurfaceFormatKHR getFormat() const;

		VkPresentModeKHR getPresentMode();

	public:
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

	private:
		VkSwapchainKHR mSwapChain;
		VkSurfaceFormatKHR mSurfaceFormat;
		VkPresentModeKHR mPresentMode;
		VkExtent2D mExtent;

		// amount of images the swapchain should store
		static constexpr int mSwapchainImageAmount = 1;
	};
}