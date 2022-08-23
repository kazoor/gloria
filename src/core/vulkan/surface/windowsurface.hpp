#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../defines.hpp"
#include "../../window/window.hpp"

namespace gloria::core {
	class WindowSurface {
	public:
		WindowSurface() {}

		WindowSurface(VkInstance instance, Window window) {
			createWindowSurface(instance, window);
		}
		~WindowSurface() {}

		void createWindowSurface(VkInstance instance, Window window) {
			VK_VALIDATE(glfwCreateWindowSurface(instance, window.getWindowPtr(), nullptr, &m_surface) != VK_SUCCESS, "Failed to create a window surface");
		}

		void destroy(VkInstance instance) {
			vkDestroySurfaceKHR(instance, m_surface, nullptr);
		}

		VkSurfaceKHR getSurface() {
			return m_surface;
		}

	private:
		VkSurfaceKHR m_surface;
	};
}