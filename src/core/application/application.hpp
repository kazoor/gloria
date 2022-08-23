#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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
			m_layers->setupDebugMessenger(m_vkInstance, m_debugMessenger);
			m_PhysicalDevice = PhysicalDevice(m_vkInstance);
			m_LogicalDevice = LogicalDevice(m_PhysicalDevice, *m_layers);
		}

		void mainLoop() {
			while (!glfwWindowShouldClose(m_window.get()->getWindowPtr())) {
				glfwPollEvents();
			}
		}

		void cleanup() {
			vkDestroyDevice(m_LogicalDevice.getDevice(), nullptr);

			if (m_layers->isEnabled()) {
				m_layers->DestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);
			}

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

				m_layers->populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
			else {
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			VK_VALIDATE(vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS, "Failed to create a Vulkan instance!");
		}

	private:
		std::unique_ptr<Window> m_window;
		VkInstance m_vkInstance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		ValidationLayer* m_layers;
		PhysicalDevice m_PhysicalDevice;
		LogicalDevice m_LogicalDevice;
	};

}