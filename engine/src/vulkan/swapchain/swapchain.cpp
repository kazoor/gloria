#include "swapchain.hpp"
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>
#include <limits>
#include <algorithm>
#include "../../core/instance/instance.hpp"
#include "../physicaldevice/physicaldevice.hpp"
#include "../../defines.hpp"

namespace gloria::vk {
	SwapChain::SwapChain() {
	}

	SwapChain::~SwapChain() {
	}

	void SwapChain::init() {
		createSwapChain();
		createImageViews();
	}

	void SwapChain::createSwapChain() {
		SwapChainSupportDetails swapChainSupport = core::Instance::get().getVkInstance().getPhysicalDevice().querySwapChainSupport(
			core::Instance::get().getVkInstance().getPhysicalDevice().get());

		VkSurfaceFormatKHR surfaceFormat = selectSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = selectSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = selectSwapExtent(swapChainSupport.capabilities);

		std::uint32_t imageCount = swapChainSupport.capabilities.minImageCount + mSwapChainImageAmount; // min amount + the amount we want

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = core::Instance::get().getVkInstance().getSurface().get(),
			.minImageCount = imageCount,
			.imageFormat = surfaceFormat.format,
			.imageColorSpace = surfaceFormat.colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		};

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		QueueFamilyIndices indices = findQueueFamilies(core::Instance::get().getVkInstance().getPhysicalDevice().get());
		std::uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_VALIDATE(vkCreateSwapchainKHR(core::Instance::get().getVkInstance().getLogicalDevice().get(), &createInfo, nullptr, &mSwapChain), "Failed to create swapchain");

		vkGetSwapchainImagesKHR(core::Instance::get().getVkInstance().getLogicalDevice().get(), mSwapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(core::Instance::get().getVkInstance().getLogicalDevice().get(), mSwapChain, &imageCount, swapChainImages.data());

#ifdef DEBUG
		if (mSwapChain != VK_NULL_HANDLE)
			GL_CORE_INFO("Created swapchain");
#endif // DEBUG
	}

	void SwapChain::destroy() {
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(core::Instance::get().getVkInstance().getLogicalDevice().get(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(core::Instance::get().getVkInstance().getLogicalDevice().get(), mSwapChain, nullptr);
	}

	VkSwapchainKHR& SwapChain::get() {
		return mSwapChain;
	}

	void SwapChain::createImageViews() {
		swapChainImageViews.resize(swapChainImages.size());

		for (std::size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo = {
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = swapChainImages[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = swapChainImageFormat,
				.components = {
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY
				},
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			};
			VK_VALIDATE(vkCreateImageView(core::Instance::get().getVkInstance().getLogicalDevice().get(), &createInfo, nullptr, &swapChainImageViews[i]), "Failed to create image views");

#ifdef DEBUG
			if (swapChainImageViews[i] != VK_NULL_HANDLE)
				GL_CORE_INFO("Created image view");
#endif // DEBUG
		}
	}

	void SwapChain::createSyncObjects(VkDevice device) {
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = {
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};

		VkFenceCreateInfo fenceInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VK_VALIDATE(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) ||
				vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]), "Failed to create semaphores");
		}
	}

	void SwapChain::recreateSwapchain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(core::Instance::get().getWindow().getRawWindow(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(core::Instance::get().getWindow().getRawWindow(), &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(core::Instance::get().getVkInstance().getLogicalDevice().get());

		destroy();

		createSwapChain();
		createImageViews();
	}

	// check for our wanted surface format, if it cant be found we just return the best one.
	VkSurfaceFormatKHR SwapChain::selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	// check for our wanted presentmode, if it cant be found we just return the standard one that always exists.
	VkPresentModeKHR SwapChain::selectSwapPresentMode(const std::vector< VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;

			glfwGetFramebufferSize(core::Instance::get().getWindow().getRawWindow(), &width, &height);

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