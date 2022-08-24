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
		Swapchain() {}

		Swapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface) {
			createSwapchain(physicalDevice, device, surface);
		}

		~Swapchain() {

		}

		void createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, WindowSurface surface) {
			SwapchainSupport swapchainSupport(physicalDevice, surface.getSurface());
			auto swapchainDetails = swapchainSupport.getSwapchainSupportDetails();

			m_surfaceFormat = swapchainSupport.selectSwapSurfaceFormat(swapchainDetails.m_formats);
			m_presentMode = swapchainSupport.selectSwapPresentMode(swapchainDetails.m_presentModes);
			m_extent = swapchainSupport.selectSwapExtent(swapchainDetails.m_capabilities);

			std::uint32_t imageCount = swapchainDetails.m_capabilities.minImageCount + 1;
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
			std::uint32_t queueFamilyIndices[] = { indices.getGraphicsFamily().value(), indices.getPresentFamily().value() };

			if (indices.getGraphicsFamily() != indices.getPresentFamily()) {
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

			VK_VALIDATE(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS, "Failed to create swapchain");

			vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, nullptr);
			m_swapchainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, m_swapchainImages.data());
		}

		void destroy(VkDevice device) {
			vkDestroySwapchainKHR(device, m_swapChain, nullptr);
		}

		VkSwapchainKHR getSwapchain() {
			return m_swapChain;
		}

	private:
		VkSwapchainKHR m_swapChain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D m_extent;

		std::vector<VkImage> m_swapchainImages;
	};
}