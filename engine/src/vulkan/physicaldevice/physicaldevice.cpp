#include "physicaldevice.hpp"
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
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

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, core::Instance::get().getVkInstance().getSurface().get(), &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
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

	void PhysicalDevice::init() {
		SelectPhysicalDevice();
	}

	void PhysicalDevice::SelectPhysicalDevice() {
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(core::Instance::get().getVkInstance().get(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Failed to find GPU's with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(core::Instance::get().getVkInstance().get(), &deviceCount, devices.data());

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

	VkPhysicalDevice& PhysicalDevice::get() {
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

		bool extensionsSupported = checkDeviceExtensionSupport(device);
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

			score += 1;
		}

		if (swapChainAdequate)
			score += 1;

		return score;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		std::uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, core::Instance::get().getVkInstance().getSurface().get(), &details.capabilities);

		std::uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, core::Instance::get().getVkInstance().getSurface().get(), &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, core::Instance::get().getVkInstance().getSurface().get(), &formatCount, details.formats.data());
		}

		std::uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, core::Instance::get().getVkInstance().getSurface().get(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, core::Instance::get().getVkInstance().getSurface().get(), &presentModeCount, details.presentModes.data());
		}
		return details;
	}
}