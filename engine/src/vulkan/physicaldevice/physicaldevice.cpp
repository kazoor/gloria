#include "physicaldevice.hpp"
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <map>
#include "../../core/instance/instance.hpp"

namespace gloria::vk {
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		std::uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	PhysicalDevice::PhysicalDevice() {
	}

	PhysicalDevice::~PhysicalDevice() {
	}

	void PhysicalDevice::SelectPhysicalDevice() {
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(core::Instance::get().getVkInstance().getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Failed to find GPU's with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(core::Instance::get().getVkInstance().getInstance(), &deviceCount, devices.data());

		// rate all the devices found
		std::map<int, VkPhysicalDevice> candidateDevices;
		int score = 0;
		for (const auto& device : devices) {
			score = rateDevice(device);
			candidateDevices.insert(std::make_pair(score, device));
		}

		// select the top rated gpu
		if (candidateDevices.rbegin()->first > 0) {
			mPhysicalDevice = candidateDevices.rbegin()->second;
		}
		else {
			throw std::runtime_error("Failed to find a suitable GPU");
		}

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(mPhysicalDevice, &deviceProperties);

		std::string deviceName = deviceProperties.deviceName;

		GpuInfo gpuInfo = {
			.apiVersion = deviceProperties.apiVersion,
			.driverVersion = deviceProperties.driverVersion,
			.vendorID = deviceProperties.vendorID,
			.deviceID = deviceProperties.deviceID,
			.deviceType = static_cast<std::uint32_t>(deviceProperties.deviceType),
			.deviceName = deviceName,
			.pipelineCacheUUID = deviceProperties.pipelineCacheUUID,
			.limits = deviceProperties.limits,
			.sparseProperties = deviceProperties.sparseProperties,
			.score = score
		};

		mGpuInfo = gpuInfo;
	}

	VkPhysicalDevice& PhysicalDevice::getPhysicalDevice() {
		return mPhysicalDevice;
	}

	GpuInfo& PhysicalDevice::getGpuInfo() {
		return mGpuInfo;
	}

	int PhysicalDevice::rateDevice(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilies(device);

		VkPhysicalDeviceProperties deviceProperties;

		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 2;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			score += 1;

		if (indices.isComplete())
			score += 1;

		score += deviceProperties.limits.maxImageDimension2D;

		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}
}