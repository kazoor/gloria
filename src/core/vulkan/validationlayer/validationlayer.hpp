#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "../../defines.hpp"

class ValidationLayer {
public:
	ValidationLayer() {}

	bool checkValidationLayerSupport() {
		std::uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const auto& layer : m_validationLayers) {
			bool found = false;

			for (const auto& properties : layers) {
				if (!strcmp(layer, properties.layerName)) {
					found = true;
					break;
				}
			}

			if (!found)
				return false;
		}

		return true;
	}

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_enabled) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
public:
	bool isEnabled() {
		return m_enabled;
	}

	const std::vector<const char*> m_validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

private:
#ifdef DEBUG
	const bool m_enabled = true;
#else
	const bool m_enabled = false;
#endif
};