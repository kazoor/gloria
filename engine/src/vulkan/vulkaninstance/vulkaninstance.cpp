#include "vulkaninstance.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "../../defines.hpp"
#include "../validationlayers/validationlayers.hpp"
#include "../physicaldevice/physicaldevice.hpp"

namespace gloria::vk {
	VulkanInstance::VulkanInstance() {
		mValidationLayers = std::make_shared<ValidationLayers>();
		mPhysicalDevice = std::make_shared<PhysicalDevice>();
	}

	VulkanInstance::~VulkanInstance() {
	}

	void VulkanInstance::createInstance() {
		if (mValidationLayers.get()->isEnabled() && !mValidationLayers.get()->checkValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but not available");
		}

		VkApplicationInfo appInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Gloria",
			.applicationVersion = VK_API_VERSION_1_3,
			.pEngineName = "Gloria",
			.engineVersion = VK_API_VERSION_1_3,
			.apiVersion = VK_API_VERSION_1_3
		};

		auto extensions = getRequiredExtensions();

		VkInstanceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &appInfo,
			.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (mValidationLayers.get()->isEnabled()) {
			createInfo.enabledLayerCount = static_cast<std::uint32_t>(mValidationLayers.get()->validationLayers.size());
			createInfo.ppEnabledLayerNames = mValidationLayers.get()->validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		VK_VALIDATE(vkCreateInstance(&createInfo, nullptr, &mVkInstance), "Failed to create Vulkan instance");

#ifdef DEBUG
		if (mVkInstance != VK_NULL_HANDLE)
			GL_CORE_INFO("Vulkan instance created");
#endif // DEBUG

		setupDebugMessenger();
		mPhysicalDevice.get()->SelectPhysicalDevice();

#ifdef DEBUG
		auto& gpuInfo = mPhysicalDevice.get()->getGpuInfo();
		GL_CORE_INFO("GPU selected: {0}(Driver version: {1}, API Version: {2}), Score during device selection: {3}", gpuInfo.deviceName, gpuInfo.driverVersion, gpuInfo.apiVersion, gpuInfo.score);
#endif // DEBUG
	}

	void VulkanInstance::run() {
	}

	void VulkanInstance::destroy() {
		if (mValidationLayers.get()->isEnabled()) {
			mValidationLayers.get()->DestroyDebugUtilsMessengerEXT(mVkInstance, mDebugMessenger, nullptr);
		}

		vkDestroyInstance(mVkInstance, nullptr);
	}

	VkInstance& VulkanInstance::getInstance() {
		return mVkInstance;
	}

	std::vector<const char*> VulkanInstance::getRequiredExtensions() {
		std::uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (mValidationLayers.get()->isEnabled()) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanInstance::setupDebugMessenger() {
		if (!mValidationLayers.get()->isEnabled())
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		VK_VALIDATE(mValidationLayers.get()->CreateDebugUtilsMessengerEXT(mVkInstance, &createInfo, nullptr, &mDebugMessenger), "Failed to create debug messenger");
	}

	void VulkanInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		if (bIsVerbose)
			createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = mValidationLayers.get()->debugCallback;
	}
}