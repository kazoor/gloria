#include "physicaldevice.hpp"
#include <iostream>
#include <vulkan/vulkan.h>
#include <map>
#include <set>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../surface/windowsurface.hpp"
#include "../swapchain/swapchainsupport.hpp"

namespace gloria::core {
	PhysicalDevice::PhysicalDevice() {}

	PhysicalDevice::PhysicalDevice(VkInstance instance, WindowSurface surface) {
		selectPhysicalDevice(instance, surface);
	}

	PhysicalDevice::~PhysicalDevice() {}

	void PhysicalDevice::selectPhysicalDevice(VkInstance instance, WindowSurface surface) {
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		// if no devices with enough support exist we might as well shutdown.
		if (deviceCount == 0) {
			throw std::runtime_error("Failed to find a GPU with Vulkan support!");
		}

		// allocate a vector with all GPUs that were found.
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// rate get all GPUs and rate them
		std::map<int, VkPhysicalDevice> candidateDevices;
		for (const auto& device : devices) {
			int score = rateDevice(device, surface);
			candidateDevices.insert(std::make_pair(score, device));
		}

		if (candidateDevices.rbegin()->first > 0) {
			mPhysicalDevice = candidateDevices.rbegin()->second;
		}
		else {
			throw std::runtime_error("Failed to find a suitable GPU");
		}

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(mPhysicalDevice, &deviceProperties);

		std::string deviceName = deviceProperties.deviceName;

		mGpuInfo.apiVersion = deviceProperties.apiVersion;
		mGpuInfo.driverVersion = deviceProperties.driverVersion;
		mGpuInfo.vendorID = deviceProperties.vendorID;
		mGpuInfo.deviceID = deviceProperties.deviceID;
		mGpuInfo.deviceType = static_cast<std::uint32_t>(deviceProperties.deviceType);
		mGpuInfo.deviceName = deviceName;
		mGpuInfo.pipelineCacheUUID = deviceProperties.pipelineCacheUUID;
		mGpuInfo.limits = deviceProperties.limits;
		mGpuInfo.sparseProperties = deviceProperties.sparseProperties;
	}

	VkPhysicalDevice PhysicalDevice::getPhysicalDevice() {
		return mPhysicalDevice;
	}

	int PhysicalDevice::rateDevice(VkPhysicalDevice device, WindowSurface surface) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		QueueFamilyIndices indices(device, surface);

		int score = 0;

		if (indices.isComplete())
			score += 100;

		bool swapchainAdequate = false;
		if (checkDeviceExtensionSupport(device)) {
			SwapchainSupport swapchainSupport(device, surface.getSurface());
			swapchainAdequate = !swapchainSupport.getSwapchainSupportDetails().formats.empty() && !swapchainSupport.getSwapchainSupportDetails().presentModes.empty();
		}

		if (swapchainAdequate)
			score += 100;

		// ALWAYS prioritize discrete GPUs over integrated graphics
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			score += 100;

		// maximum amount of textures possible
		score += deviceProperties.limits.maxImageDimension2D;

		// if geometry shaders arent supported theres no point in even continuing
		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		std::uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(g_deviceExtensions.begin(), g_deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	GpuInfo PhysicalDevice::getGpuInfo() {
		return mGpuInfo;
	}
}