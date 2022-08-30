#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>

namespace gloria::vk {
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct QueueFamilyIndices {
		std::optional<std::uint32_t> graphicsFamily;
		std::optional<std::uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	// struct used to parse GPU info from the chosen physical device.
	// based on VkPhysicalDeviceProperties
	struct GpuInfo {
		std::uint32_t apiVersion;
		std::uint32_t driverVersion;
		std::uint32_t vendorID;
		std::uint32_t deviceID;
		std::uint32_t deviceType;
		std::string deviceName;
		uint8_t* pipelineCacheUUID;
		VkPhysicalDeviceLimits limits;
		VkPhysicalDeviceSparseProperties sparseProperties;
		int score;
	};

	class PhysicalDevice {
	public:
		PhysicalDevice();

		~PhysicalDevice();

		void init();

		void SelectPhysicalDevice();

		VkPhysicalDevice& get();

		GpuInfo& getGpuInfo();


		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	private:
		// rates all GPU's available by a score and selects the best one
		int rateDevice(VkPhysicalDevice device);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };

		GpuInfo mGpuInfo;
	};
}
