#include "logicaldevice.hpp"
#include "../../defines.hpp"

namespace gloria::core {
	LogicalDevice::LogicalDevice() {}

	LogicalDevice::LogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer validationLayers, WindowSurface surface) {
		createLogicalDevice(physicalDevice, validationLayers, surface);
	}

	LogicalDevice::~LogicalDevice() {}

	void LogicalDevice::createLogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer validationLayers, WindowSurface surface) {
		QueueFamilyIndices indices(physicalDevice.getPhysicalDevice(), surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<std::uint32_t> uniqueQueueFamilies = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };
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

		VkPhysicalDeviceVulkan13Features dynamicRendering = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
			.dynamicRendering = VK_TRUE
		};

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.pNext = &dynamicRendering;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();

		if (validationLayers.isEnabled()) {
			createInfo.enabledLayerCount = static_cast<std::uint32_t>(g_validationLayers.size());
			createInfo.ppEnabledLayerNames = g_validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		VK_VALIDATE(vkCreateDevice(physicalDevice.getPhysicalDevice(), &createInfo, nullptr, &m_device), "failed to create logical device!");

		vkGetDeviceQueue(m_device, indices.m_graphicsFamily.value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, indices.m_presentFamily.value(), 0, &m_presentQueue);
	}

	void LogicalDevice::destroy() {
		vkDestroyDevice(m_device, nullptr);
	}

	VkDevice LogicalDevice::getDevice() {
		return m_device;
	}

	VkQueue LogicalDevice::getGraphicsQueue()
	{
		return m_graphicsQueue;
	}

	VkQueue LogicalDevice::getPresentQueue()
	{
		return m_presentQueue;
	}
}