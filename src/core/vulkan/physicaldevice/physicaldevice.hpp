#pragma once
#include <iostream>
#include <vulkan/vulkan.h>
#include <map>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	class PhysicalDevice {
	public:
		PhysicalDevice() {}

		PhysicalDevice(VkInstance instance, WindowSurface surface) {
			selectPhysicalDevice(instance, surface);
		}

		~PhysicalDevice() {}

		void selectPhysicalDevice(VkInstance instance, WindowSurface surface) {
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
				m_PhysicalDevice = candidateDevices.rbegin()->second;
			}
			else {
				throw std::runtime_error("Failed to find a suitable GPU");
			}

#ifdef DEBUG
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
			GL_CORE_INFO("DEVICES: {0}, DEVICE_NAME: {1}", devices.size(), deviceProperties.deviceName);
#endif // DEBUG
		}

		VkPhysicalDevice getPhysicalDevice() {
			return m_PhysicalDevice;
		}

	private:
		// rates a gpu by
		int rateDevice(VkPhysicalDevice device, WindowSurface surface) {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			QueueFamilyIndices indices(device, surface);

			int score = 0;

			if (indices.isComplete())
				score += indices.getGraphicsFamily().value();

			// ALWAYS prioritize discrete GPUs over integrated graphics
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000;

			// maximum amount of textures possible
			score += deviceProperties.limits.maxImageDimension2D;

			// if geometry shaders arent supported theres no point in even continuing
			if (!deviceFeatures.geometryShader) {
				return 0;
			}

			return score;
		}
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	};
}