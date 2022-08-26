#include "swapchain.hpp"
#include <iostream>
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "swapchainsupport.hpp"
#include "../surface/windowsurface.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"

namespace gloria::core {
	Swapchain::Swapchain() {}

	Swapchain::Swapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface) {
		createSwapchain(physicalDevice, device, surface);
	}

	Swapchain::~Swapchain() {

	}

	void Swapchain::createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface) {
		SwapchainSupport swapchainSupport(physicalDevice, surface.getSurface());
		auto swapchainDetails = swapchainSupport.getSwapchainSupportDetails();

		mSurfaceFormat = swapchainSupport.selectSwapSurfaceFormat(swapchainDetails.formats);
		mPresentMode = swapchainSupport.selectSwapPresentMode(swapchainDetails.presentModes);
		mExtent = swapchainSupport.selectSwapExtent(swapchainDetails.capabilities);

		std::uint32_t imageCount = swapchainDetails.capabilities.minImageCount + mSwapchainImageAmount;
		if (swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount) {
			imageCount = swapchainDetails.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface.getSurface(),
			.minImageCount = imageCount,
			.imageFormat = mSurfaceFormat.format,
			.imageColorSpace = mSurfaceFormat.colorSpace,
			.imageExtent = mExtent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		};

		QueueFamilyIndices indices(physicalDevice, surface);
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

		createInfo.preTransform = swapchainDetails.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = mPresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_VALIDATE(vkCreateSwapchainKHR(device, &createInfo, nullptr, &mSwapChain), "Failed to create swapchain");

		vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, swapchainImages.data());
	}

	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
		VkImageViewCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = {
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		VkImageView imageView;
		VK_VALIDATE(vkCreateImageView(device, &createInfo, nullptr, &imageView), "Failed to create image views!");

		return imageView;
	}

	void Swapchain::CreateSwapchainImageViews(VkDevice device) {
		swapchainImageViews.resize(swapchainImages.size());
		for (int i = 0; i < swapchainImages.size(); ++i) {
			swapchainImageViews[i] = createImageView(device, swapchainImages[i], mSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	void Swapchain::createSyncObjects(VkDevice device) {
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

		for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			VK_VALIDATE(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) ||
				vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]), "Failed to create semaphores");
		}
	}

	void Swapchain::destroy(VkDevice device) {
		vkDestroySwapchainKHR(device, mSwapChain, nullptr);
	}

	VkSwapchainKHR Swapchain::getSwapchain() {
		return mSwapChain;
	}

	VkExtent2D Swapchain::getExtent() {
		return mExtent;
	}

	VkSurfaceFormatKHR Swapchain::getFormat() const {
		return mSurfaceFormat;
	}

	VkPresentModeKHR Swapchain::getPresentMode() {
		return mPresentMode;
	}
}