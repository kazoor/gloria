#include "logicaldevice.hpp"
#include <vulkan/vulkan.h>
#include <set>
#include "../physicaldevice/physicaldevice.hpp"
#include "../../core/instance/instance.hpp"
#include "../../defines.hpp"

namespace gloria::vk {
	LogicalDevice::LogicalDevice() {
	}

	LogicalDevice::~LogicalDevice() {
	}

	void LogicalDevice::init() {
		createLogicalDevice();
	}

	void LogicalDevice::createLogicalDevice() {
		QueueFamilyIndices indices = findQueueFamilies(core::Instance::get().getVkInstance().getPhysicalDevice().get());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<std::uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		float queuePrio = 1.0f;


		for (std::uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = queueFamily,
				.queueCount = 1,
				.pQueuePriorities = &queuePrio
			};

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledExtensionCount = 0,
			.pEnabledFeatures = &deviceFeatures,
		};

		if (core::Instance::get().getVkInstance().getValidationLayers().isEnabled()) {
			createInfo.enabledLayerCount = static_cast<std::uint32_t>(core::Instance::get().getVkInstance().getValidationLayers().validationLayers.size());
			createInfo.ppEnabledLayerNames = core::Instance::get().getVkInstance().getValidationLayers().validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		VK_VALIDATE(vkCreateDevice(core::Instance::get().getVkInstance().getPhysicalDevice().get(), &createInfo, nullptr, &mDevice), "Failed to create logical device");

#ifdef DEBUG
		if (mDevice != VK_NULL_HANDLE)
			GL_CORE_INFO("Logical device created");
#endif // DEBUG

		vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);

		vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
	}

	void LogicalDevice::destroy() {
		vkDestroyDevice(mDevice, nullptr);
	}

	VkDevice& LogicalDevice::get() {
		return mDevice;
	}

	VkQueue& LogicalDevice::getGraphicsQueue() {
		return mGraphicsQueue;
	}
}