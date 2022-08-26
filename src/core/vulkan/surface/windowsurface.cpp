#include "windowsurface.hpp"

namespace gloria::core {
	WindowSurface::WindowSurface() {}

	WindowSurface::WindowSurface(VkInstance instance, Window window) {
		createWindowSurface(instance, window);
	}
	WindowSurface::~WindowSurface() {}

	void WindowSurface::createWindowSurface(VkInstance instance, Window window) {
		VK_VALIDATE(glfwCreateWindowSurface(instance, window.getWindowPtr(), nullptr, &m_surface), "Failed to create a window surface");
	}

	void WindowSurface::destroy(VkInstance instance) {
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
	}

	VkSurfaceKHR WindowSurface::getSurface() {
		return m_surface;
	}
}