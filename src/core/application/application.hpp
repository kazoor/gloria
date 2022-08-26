#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <vector>
#include "../defines.hpp"
#include "../window/window.hpp"
#include "../vulkan/validationlayer/validationlayer.hpp"
#include "../logger/logger.hpp"
#include "../vulkan/physicaldevice/physicaldevice.hpp"
#include "../vulkan/logicaldevice/logicaldevice.hpp"
#include "../vulkan/surface/windowsurface.hpp"
#include "../vulkan/swapchain/swapchain.hpp"
#include "../vulkan/graphicspipeline/graphicspipeline.hpp"
#include "../vulkan/commandbuffers/commandbuffers.hpp"
#include "../vulkan/vulkanrenderer/vulkanrenderer.hpp"

inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

inline VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	switch (messageSeverity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
		GL_CORE_TRACE("{0}", pCallbackData->pMessage);
		break;
	}
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
		GL_CORE_INFO("{0}", pCallbackData->pMessage);
		break;
	}

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
		GL_CORE_WARN("{0}", pCallbackData->pMessage);
		break;
	}

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
		GL_CORE_ERROR("{0}", pCallbackData->pMessage);
		break;
	}

	default: break;
	}

	return VK_FALSE;
}

namespace gloria::core {

	class Application {
	public:
		Application();

		~Application();

		void run();

	private:
		void initWindow();

		void initVulkan();

		void mainLoop();

		void cleanup();

		void createVulkanInstance();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void setupDebugMessenger();

	private:
		std::unique_ptr<Window> pWindow;
		VkInstance mVkInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		ValidationLayer mLayers;
		WindowSurface mSurface;
		PhysicalDevice mPhysicalDevice;
		LogicalDevice mLogicalDevice;
		Swapchain mSwapchain;
		GraphicsPipeline mGraphicsPipeline;
		CommandPool mCommandPool;
		CommandBuffer mCommandBuffer;
		VulkanRenderer mRenderer;
	};
}