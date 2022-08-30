#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../../utils/base.hpp"
#include "../validationlayers/validationlayers.hpp"
#include "../physicaldevice/physicaldevice.hpp"
#include "../logicaldevice/logicaldevice.hpp"
#include "../surface/surface.hpp"
#include "../swapchain/swapchain.hpp"

namespace gloria::vk {
	class VulkanInstance {
	public:
		VulkanInstance();

		~VulkanInstance();

		void init();

		void createInstance();

		void run();

		void destroy();

		VkInstance& get();

		PhysicalDevice& getPhysicalDevice();

		ValidationLayers& getValidationLayers();

		LogicalDevice& getLogicalDevice();

		Surface& getSurface();

	private:
		std::vector<const char*> getRequiredExtensions();

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkInstance mVkInstance;

		Shared<ValidationLayers> mValidationLayers;

		Shared<PhysicalDevice> mPhysicalDevice;

		Shared<LogicalDevice> mLogicalDevice;

		Shared<Surface> mSurface;

		Shared<SwapChain> mSwapchain;

		VkDebugUtilsMessengerEXT mDebugMessenger;
	};
}