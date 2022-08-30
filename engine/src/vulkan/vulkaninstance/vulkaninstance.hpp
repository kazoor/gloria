#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../../utils/base.hpp"
#include "../validationlayers/validationlayers.hpp"
#include "../physicaldevice/physicaldevice.hpp"

namespace gloria::vk {
	class VulkanInstance
	{
	public:
		VulkanInstance();

		~VulkanInstance();

		void init();

		void createInstance();

		void run();

		void destroy();

		VkInstance& getInstance();

	private:
		std::vector<const char*> getRequiredExtensions();

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkInstance mVkInstance;

		Shared<ValidationLayers> mValidationLayers;

		Shared<PhysicalDevice> mPhysicalDevice;

		VkDebugUtilsMessengerEXT mDebugMessenger;
	};
}