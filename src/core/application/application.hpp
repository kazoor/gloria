#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../defines.hpp"
#include "../window/window.hpp"
#include "../vulkan/validationlayer/validationlayer.hpp"
#include "../logger/logger.hpp"
#include "../vulkan/physicaldevice/physicaldevice.hpp"
#include "../vulkan/logicaldevice/logicaldevice.hpp"
#include "../vulkan/surface/windowsurface.hpp"

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
#ifdef DEBUG
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
		GL_CORE_TRACE("{0}", pCallbackData->pMessage);
		break;
	}
#endif // DEBUG
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
		Application() {}

		~Application() {
			cleanup();
		}

		void run() {
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		void initWindow() {
			m_window = std::make_unique<Window>(1920, 1080, "Gloria");
			Log::Init();
		}

		void initVulkan() {
			createVulkanInstance();
			setupDebugMessenger();
			m_surface = WindowSurface(m_vkInstance, *m_window);
			m_PhysicalDevice = PhysicalDevice(m_vkInstance, m_surface);
			m_LogicalDevice = LogicalDevice(m_PhysicalDevice, m_layers, m_surface);
		}

		void mainLoop() {
			while (!glfwWindowShouldClose(m_window.get()->getWindowPtr())) {
				glfwPollEvents();
			}

			vkDeviceWaitIdle(m_LogicalDevice.getDevice());
		}

		void cleanup() {
			vkDestroyDevice(m_LogicalDevice.getDevice(), nullptr);

			if (m_layers->isEnabled())
				DestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);

			m_surface.destroy(m_vkInstance);
			vkDestroyInstance(m_vkInstance, nullptr);

			glfwDestroyWindow(m_window.get()->getWindowPtr());

			glfwTerminate();

			delete m_layers;
		}

		void createVulkanInstance() {
			m_layers = new ValidationLayer();

			if (m_layers->isEnabled() && !m_layers->checkValidationLayerSupport()) {
				throw std::runtime_error("Validation layers requested, but not available!").what();
			}

			VkApplicationInfo appInfo{
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName = "Gloria",
				.applicationVersion = VK_API_VERSION_1_3,
				.pEngineName = "Gloria",
				.engineVersion = VK_API_VERSION_1_3,
				.apiVersion = VK_API_VERSION_1_3
			};

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = m_layers->getRequiredExtensions();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();

			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			if (m_layers->isEnabled()) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(m_layers->m_validationLayers.size());
				createInfo.ppEnabledLayerNames = m_layers->m_validationLayers.data();

				populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
			else {
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			VK_VALIDATE(vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS, "Failed to create a Vulkan instance!");
		}

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
		}

		void setupDebugMessenger() {
			if (!m_layers->isEnabled()) return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			populateDebugMessengerCreateInfo(createInfo);

			VK_VALIDATE(CreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS, "Failed to set up debug messenger!");
		}
	private:
		std::unique_ptr<Window> m_window;
		VkInstance m_vkInstance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		ValidationLayer* m_layers;
		WindowSurface m_surface;
		PhysicalDevice m_PhysicalDevice;
		LogicalDevice m_LogicalDevice;
	};

}