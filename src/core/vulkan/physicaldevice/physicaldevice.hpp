#pragma once
#include <iostream>
#include <string>
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../surface/windowsurface.hpp"
#include "../swapchain/swapchainsupport.hpp"

namespace gloria::core {
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

		PhysicalDevice(VkInstance instance, WindowSurface surface);

		~PhysicalDevice();

		void selectPhysicalDevice(VkInstance instance, WindowSurface surface);

		VkPhysicalDevice getPhysicalDevice();

		GpuInfo getGpuInfo();

	private:
		// rates a GPU's by features.
		int rateDevice(VkPhysicalDevice device, WindowSurface surface);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	private:
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		GpuInfo mGpuInfo;
	};
}