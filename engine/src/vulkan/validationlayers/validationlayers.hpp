#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace gloria::vk {
	class ValidationLayers {
	public:
		ValidationLayers();
		~ValidationLayers();

		bool checkValidationLayerSupport();

		bool isEnabled();

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	private:
#ifdef DEBUG
		const bool mEnabled = true;
#else
		const bool mEnabled = false;
#endif // DEBUG
	};
}