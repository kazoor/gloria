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
		std::vector<VkImage> m_swapchainImages;
		std::vector<VkImageView> m_swapchainImageViews;
		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;

	private:
		VkSwapchainKHR m_swapChain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D m_extent;

		// amount of images the swapchain should store
		static constexpr int m_swapchainImageAmount = 1;
	};
}