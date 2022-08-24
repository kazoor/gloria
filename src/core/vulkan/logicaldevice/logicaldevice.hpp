#pragma once
#include <vulkan/vulkan.h>
#include <set>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../validationlayer/validationlayer.hpp"
#include "../physicaldevice/physicaldevice.hpp"
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	class LogicalDevice {
	public:
		LogicalDevice() {}

		LogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer* validationLayers, WindowSurface surface) {
			createLogicalDevice(physicalDevice, validationLayers, surface);
		}

		~LogicalDevice() {}

		void createLogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer* validationLayers, WindowSurface surface) {
			QueueFamilyIndices indices(physicalDevice.getPhysicalDevice(), surface);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<std::uint32_t> uniqueQueueFamilies = { indices.getGraphicsFamily().value(), indices.getPresentFamily().value() };
			float queuePriority = 1.0f;

			for (std::uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo = {
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = queueFamily,
					.queueCount = 1,
					.pQueuePriorities = &queuePriority
				};

				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
			createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();

			if (validationLayers->isEnabled()) {
				createInfo.enabledLayerCount = static_cast<std::uint32_t>(g_validationLayers.size());
				createInfo.ppEnabledLayerNames = g_validationLayers.data();
			}
			else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(physicalDevice.getPhysicalDevice(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(m_device, indices.getGraphicsFamily().value(), 0, &m_graphicsQueue);
			vkGetDeviceQueue(m_device, indices.getPresentFamily().value(), 0, &m_presentQueue);
		}

		void destroy() {
			vkDestroyDevice(m_device, nullptr);
		}

		VkDevice getDevice() {
			return m_device;
		}

	private:
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
	};
}