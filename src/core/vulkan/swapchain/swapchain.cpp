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

		m_surfaceFormat = swapchainSupport.selectSwapSurfaceFormat(swapchainDetails.m_formats);
		m_presentMode = swapchainSupport.selectSwapPresentMode(swapchainDetails.m_presentModes);
		m_extent = swapchainSupport.selectSwapExtent(swapchainDetails.m_capabilities);

		std::uint32_t imageCount = swapchainDetails.m_capabilities.minImageCount + m_swapchainImageAmount;
		if (swapchainDetails.m_capabilities.maxImageCount > 0 && imageCount > swapchainDetails.m_capabilities.maxImageCount) {
			imageCount = swapchainDetails.m_capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface.getSurface(),
			.minImageCount = imageCount,
			.imageFormat = m_surfaceFormat.format,
			.imageColorSpace = m_surfaceFormat.colorSpace,
			.imageExtent = m_extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		};

		QueueFamilyIndices indices(physicalDevice, surface);
		std::uint32_t queueFamilyIndices[] = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

		if (indices.m_graphicsFamily != indices.m_presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapchainDetails.m_capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_VALIDATE(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain), "Failed to create swapchain");

		vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, nullptr);
		m_swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, m_swapchainImages.data());
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
		m_swapchainImageViews.resize(m_swapchainImages.size());
		for (int i = 0; i < m_swapchainImages.size(); ++i) {
			m_swapchainImageViews[i] = createImageView(device, m_swapchainImages[i], m_surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	void Swapchain::createSyncObjects(VkDevice device) {
		m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};

		VkFenceCreateInfo fenceInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			VK_VALIDATE(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) ||
				vkCreateFence(device, &fenceInfo, nullptr, &m_inFlightFences[i]), "Failed to create semaphores");
		}
	}

	void Swapchain::destroy(VkDevice device) {
		vkDestroySwapchainKHR(device, m_swapChain, nullptr);
	}

	VkSwapchainKHR Swapchain::getSwapchain() {
		return m_swapChain;
	}

	VkExtent2D Swapchain::getExtent() {
		return m_extent;
	}

	VkSurfaceFormatKHR Swapchain::getFormat() const {
		return m_surfaceFormat;
	}

	VkPresentModeKHR Swapchain::getPresentMode() {
		return m_presentMode;
	}
}