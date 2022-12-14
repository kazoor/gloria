#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace gloria::vk {
	class SwapChain {
	public:
		SwapChain();

		~SwapChain();

		void init();

		void createSwapChain();

		void destroy();

		VkSwapchainKHR& get();

		void createImageViews();

		void createSyncObjects(VkDevice device);

		void recreateSwapchain();

	public:
		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;

		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		std::vector<VkSemaphore> imageAvailableSemaphores;

		std::vector<VkSemaphore> renderFinishedSemaphores;

		std::vector<VkFence> inFlightFences;

		const int MAX_FRAMES_IN_FLIGHT = 2;

	private:
		// check for our wanted surface format, if it cant be found we just return the best one.
		VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		// check for our wanted presentmode, if it cant be found we just return the standard one that always exists.
		VkPresentModeKHR selectSwapPresentMode(const std::vector< VkPresentModeKHR>& availablePresentModes);

		VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		const int mSwapChainImageAmount{ 1 };

		VkSwapchainKHR mSwapChain;
	};
}