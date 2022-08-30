#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../../utils/base.hpp"
#include "../validationlayers/validationlayers.hpp"

namespace gloria::vk {
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		void createInstance();

		void run();

		void destroy();

	private:
		std::vector<const char*> getRequiredExtensions();

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkInstance mVkInstance;
		Shared<ValidationLayers> mValidationLayers;
		VkDebugUtilsMessengerEXT mDebugMessenger;
	};
}