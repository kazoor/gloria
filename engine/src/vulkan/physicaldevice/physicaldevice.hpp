#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <optional>

namespace gloria::vk {
	struct QueueFamilyIndices {
		std::optional<std::uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

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
	};

	class PhysicalDevice {
	public:
		PhysicalDevice();

		~PhysicalDevice();

		void SelectPhysicalDevice();

		VkPhysicalDevice& getPhysicalDevice();

		GpuInfo& getGpuInfo();

	private:
		// rates all GPU's available by a score and selects the best one
		int rateDevice(VkPhysicalDevice device);

		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };

		GpuInfo mGpuInfo;
	};
}