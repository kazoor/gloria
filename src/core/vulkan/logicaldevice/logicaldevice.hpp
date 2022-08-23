#pragma once
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../validationlayer/validationlayer.hpp"
#include "../physicaldevice/physicaldevice.hpp"

namespace gloria::core {
	class LogicalDevice {
	public:
		LogicalDevice() {}

		LogicalDevice(PhysicalDevice physicalDevice, ValidationLayer validationLayers) {
			createLogicalDevice(physicalDevice, validationLayers);
		}

		~LogicalDevice() {}

		void createLogicalDevice(PhysicalDevice physicalDevice, ValidationLayer validationLayers) {
			QueueFamilyIndices indices(physicalDevice.getPhysicalDevice());

			float queuePrio = 1.0f;
			VkDeviceQueueCreateInfo createQueueInfo = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = indices.getGraphicsFamily().value(),
				.queueCount = 1,
				.pQueuePriorities = &queuePrio
			};

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createDeviceInfo{};
			createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createDeviceInfo.pQueueCreateInfos = &createQueueInfo;
			createDeviceInfo.queueCreateInfoCount = 1;
			createDeviceInfo.pEnabledFeatures = &deviceFeatures;
			createDeviceInfo.enabledExtensionCount = 0;

			if (validationLayers.isEnabled()) {
				createDeviceInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.m_validationLayers.size());
				createDeviceInfo.ppEnabledLayerNames = validationLayers.m_validationLayers.data();
			}
			else {
				createDeviceInfo.enabledLayerCount = 0;
			}

			VK_VALIDATE(vkCreateDevice(physicalDevice.getPhysicalDevice(), &createDeviceInfo, nullptr, &m_device) != VK_SUCCESS, "Failed to create a LogicalDevice");

			vkGetDeviceQueue(m_device, indices.getGraphicsFamily().value(), 0, &m_graphicsQueue);
		}

		VkDevice getDevice()& {
			return m_device;
		}

	private:
		VkDevice m_device;
		VkQueue m_graphicsQueue;
	};
}