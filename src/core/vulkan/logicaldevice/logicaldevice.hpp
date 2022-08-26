#pragma once
#include <vulkan/vulkan.h>
#include "../../defines.hpp"
#include "../queuefamilyindices/queuefamilyindices.hpp"
#include "../validationlayer/validationlayer.hpp"
#include "../physicaldevice/physicaldevice.hpp"
#include "../surface/windowsurface.hpp"

namespace gloria::core {
	class LogicalDevice {
	public:
		LogicalDevice();

		LogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer validationLayers, WindowSurface surface);

		~LogicalDevice();

		void createLogicalDevice(PhysicalDevice& physicalDevice, ValidationLayer validationLayers, WindowSurface surface);

		void destroy();

		VkDevice getDevice();

		VkQueue getGraphicsQueue();

		VkQueue getPresentQueue();

	private:
		VkDevice mDevice;
		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;
	};
}