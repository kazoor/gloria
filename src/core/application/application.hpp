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
		Application() {
			Log::Init();
		}

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
		}

		void initVulkan() {
			createVulkanInstance();
			setupDebugMessenger();
			m_surface = WindowSurface(m_vkInstance, *m_window);
			m_physicalDevice = PhysicalDevice(m_vkInstance, m_surface);
			m_logicalDevice = LogicalDevice(m_physicalDevice, m_layers, m_surface);
			m_swapchain = Swapchain(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_surface);
			m_swapchain.CreateSwapchainImageViews(m_logicalDevice.getDevice());
			m_graphicsPipeline = GraphicsPipeline(m_logicalDevice.getDevice(), m_swapchain);
			m_commandPool = CommandPool(m_logicalDevice.getDevice(), m_physicalDevice.getPhysicalDevice(), m_surface);
			m_commandBuffer = CommandBuffer(m_logicalDevice.getDevice(), m_commandPool);
			m_swapchain.createSyncObjects(m_logicalDevice.getDevice());
			m_renderer = VulkanRenderer(m_logicalDevice, m_swapchain, m_graphicsPipeline, m_commandBuffer);
		}

		void mainLoop() {
			while (!glfwWindowShouldClose(m_window.get()->getWindowPtr())) {
				glfwPollEvents();
				m_renderer.drawFrame();
			}

			vkDeviceWaitIdle(m_logicalDevice.getDevice());
		}

		void cleanup() {
			vkDestroySemaphore(m_logicalDevice.getDevice(), m_swapchain.getImageAvailableSemaphore(), nullptr);
			vkDestroySemaphore(m_logicalDevice.getDevice(), m_swapchain.getRenderFinishedSemaphore(), nullptr);
			vkDestroyFence(m_logicalDevice.getDevice(), m_swapchain.getInFlightFence(), nullptr);

			m_commandPool.destroy(m_logicalDevice.getDevice());

			m_graphicsPipeline.destroy(m_logicalDevice.getDevice());

			for (const auto& imageView : m_swapchain.m_swapchainImageViews) {
				vkDestroyImageView(m_logicalDevice.getDevice(), imageView, nullptr);
			}

			m_swapchain.destroy(m_logicalDevice.getDevice());

			vkDestroyDevice(m_logicalDevice.getDevice(), nullptr);

			if (m_layers.isEnabled()) {
				DestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);
			}

			m_surface.destroy(m_vkInstance);

			vkDestroyInstance(m_vkInstance, nullptr);

			glfwDestroyWindow(m_window.get()->getWindowPtr());

			glfwTerminate();

			m_window.reset();
		}

		void createVulkanInstance() {
			if (m_layers.isEnabled() && !m_layers.checkValidationLayerSupport()) {
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

			auto extensions = m_layers.getRequiredExtensions();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();

			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			if (m_layers.isEnabled()) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
				createInfo.ppEnabledLayerNames = g_validationLayers.data();

				populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
			else {
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			VK_VALIDATE(vkCreateInstance(&createInfo, nullptr, &m_vkInstance), "Failed to create a Vulkan instance!");
		}

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
		}

		void setupDebugMessenger() {
			if (!m_layers.isEnabled()) return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			populateDebugMessengerCreateInfo(createInfo);

			VK_VALIDATE(CreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_debugMessenger), "Failed to set up debug messenger!");
		}

	private:
		std::unique_ptr<Window> m_window;
		VkInstance m_vkInstance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		ValidationLayer m_layers;
		WindowSurface m_surface;
		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;
		Swapchain m_swapchain;
		GraphicsPipeline m_graphicsPipeline;
		CommandPool m_commandPool;
		CommandBuffer m_commandBuffer;
		VulkanRenderer m_renderer;
	};
}