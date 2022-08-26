#include "Application.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
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

namespace gloria::core {
	Application::Application() {
		Log::Init();
	}

	Application::~Application() {
		cleanup();
	}

	void Application::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void Application::initWindow() {
		pWindow = std::make_unique<Window>(1920, 1080, "Gloria");
	}

	void Application::initVulkan() {
		createVulkanInstance();
		setupDebugMessenger();
		mSurface = WindowSurface(mVkInstance, *pWindow);
		mPhysicalDevice = PhysicalDevice(mVkInstance, mSurface);
		mLogicalDevice = LogicalDevice(mPhysicalDevice, mLayers, mSurface);
		mSwapchain = Swapchain(mPhysicalDevice.getPhysicalDevice(), mLogicalDevice.getDevice(), mSurface);
		mSwapchain.CreateSwapchainImageViews(mLogicalDevice.getDevice());
		mGraphicsPipeline = GraphicsPipeline(mLogicalDevice.getDevice(), mSwapchain);
		mCommandPool = CommandPool(mLogicalDevice.getDevice(), mPhysicalDevice.getPhysicalDevice(), mSurface);
		mCommandBuffer = CommandBuffer(mLogicalDevice.getDevice(), mCommandPool);
		mSwapchain.createSyncObjects(mLogicalDevice.getDevice());
		mRenderer = VulkanRenderer(mLogicalDevice, mSwapchain, mGraphicsPipeline, mCommandBuffer);

		auto gpuInfo = mPhysicalDevice.getGpuInfo();

		GL_CORE_INFO("Selected GPU: {0}(apiVersion: {1}, driverVersion: {2}, vendorID: {3}, deviceID: {4}, deviceType: {5})",
			gpuInfo.deviceName, gpuInfo.apiVersion, 
			gpuInfo.driverVersion, gpuInfo.vendorID,
			gpuInfo.deviceID, gpuInfo.deviceType);
	}

	void Application::mainLoop() {
		while (!glfwWindowShouldClose(pWindow.get()->getWindowPtr())) {
			glfwPollEvents();
			mRenderer.drawFrame();
		}

		vkDeviceWaitIdle(mLogicalDevice.getDevice());
	}

	void Application::cleanup() {
		for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			vkDestroySemaphore(mLogicalDevice.getDevice(), mSwapchain.imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(mLogicalDevice.getDevice(), mSwapchain.renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(mLogicalDevice.getDevice(), mSwapchain.inFlightFences[i], nullptr);
		}

		mCommandPool.destroy(mLogicalDevice.getDevice());

		mGraphicsPipeline.destroy(mLogicalDevice.getDevice());

		for (const auto& imageView : mSwapchain.swapchainImageViews) {
			vkDestroyImageView(mLogicalDevice.getDevice(), imageView, nullptr);
		}

		mSwapchain.destroy(mLogicalDevice.getDevice());

		vkDestroyDevice(mLogicalDevice.getDevice(), nullptr);

		if (mLayers.isEnabled()) {
			DestroyDebugUtilsMessengerEXT(mVkInstance, mDebugMessenger, nullptr);
		}

		mSurface.destroy(mVkInstance);

		vkDestroyInstance(mVkInstance, nullptr);

		glfwDestroyWindow(pWindow.get()->getWindowPtr());

		glfwTerminate();

		pWindow.reset();
	}

	void Application::createVulkanInstance() {
		if (mLayers.isEnabled() && !mLayers.checkValidationLayerSupport()) {
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

		auto extensions = mLayers.getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (mLayers.isEnabled()) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
			createInfo.ppEnabledLayerNames = g_validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		VK_VALIDATE(vkCreateInstance(&createInfo, nullptr, &mVkInstance), "Failed to create a Vulkan instance!");

#ifdef DEBUG
		if (mVkInstance != VK_NULL_HANDLE)
			GL_CORE_INFO("mVkInstance has been created");
#endif // DEBUG

	}

	void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void Application::setupDebugMessenger() {
		if (!mLayers.isEnabled()) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		VK_VALIDATE(CreateDebugUtilsMessengerEXT(mVkInstance, &createInfo, nullptr, &mDebugMessenger), "Failed to set up debug messenger!");

#ifdef DEBUG
		if (mDebugMessenger != VK_NULL_HANDLE)
			GL_CORE_INFO("mDebugMessenger has been created");
#endif // DEBUG
	}

}